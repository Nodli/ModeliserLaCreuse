#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include <queue>
#include <iostream>

#include <Weather/Erosion.hpp>
#include <BooleanField.hpp>
#include <Utils.hpp>

void erode_using_median_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	Eigen::Vector2i positions[8];
	double values[8];
	double slopes[8];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			// getting neighbor slope
			int neighbors = layers.neighbors_info(w, h, values, positions, slopes);

			// computing the median slope
			abs_array(neighbors, slopes);
			double median_slope = median_array(neighbors, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * median_slope;
			layers.get_field(1).at(w, h) += k * median_slope;
		}
	}
}

void erode_using_mean_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	Eigen::Vector2i positions[8];
	double values[8];
	double slopes[8];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			// getting neighbor slope
			int neighbors = layers.neighbors_info(w, h, values, positions, slopes);

			// computing the median slope
			abs_array(neighbors, slopes);
			double mean_slope = mean_array(neighbors, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * mean_slope;
			layers.get_field(1).at(w, h) += k * mean_slope;
		}
	}
}

void erode_constant(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	for(int h = 0; h < layers.grid_height(); ++h)
	{
		for(int w = 0; w < layers.grid_width(); ++w)
		{
			layers.get_field(0).at(w, h) -= k; // berock
			layers.get_field(1).at(w, h) += k; // sediments
		}
	}

}

float erosion_control_function(const double slope){

	// max slope to consider
	float max_slope_threshold = 40.;

	// fall-off function taken from implicit function ray-marching
	float slope_remap = slope / max_slope_threshold;
	float temp_value = 1. - slope_remap * slope_remap;

	return temp_value * temp_value * temp_value;

}

void erode_slope_controled(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	for(int j = 0; j < layers.grid_height(); ++j){
		for(int i = 0; i < layers.grid_width(); ++i){
			double delta_value = k * erosion_control_function(layers.get_field(0).slope(i, j));

			layers.get_field(0).at(i, j) -= delta_value;
			layers.get_field(1).at(i, j) += delta_value;
		}
	}

}

void erode_and_transport(MultiLayerMap& layers, const double k, const int iteration_max, const double rest_angle)
{
	// creating erosion layer if needed
	if(layers.get_layer_number() == 1)
	{
		layers.new_layer();
	}

	double slope_stability_threshold = layers.cell_size().x() * tan(rest_angle / 180. * 3.14);
	std::cout << "slope_stability_threshold: " << slope_stability_threshold << std::endl;

	double values[8];
	Eigen::Vector2i positions[8];
	double slopes[8];
	double slopes_proportions[8];

	for(int s = 0; s < iteration_max; ++s)
	{

		std::cout << "iteration: " << s << std::endl;

		// conversion bedrock -> sediments
		erode_constant(layers, k);

		SimpleLayerMap terrain = layers.generate_field();

		// all cells are considered unstable at initialization
		BooleanField unstable(layers.grid_width(), layers.grid_height(), true);

		std::vector<Eigen::Vector2i> coord_vector;
		for(int i = 0; i < terrain.grid_height(); ++i){
			for(int j = 0; j < terrain.grid_width(); ++j){
				coord_vector.push_back({i, j});
			}
		}
		// std::random_shuffle(coord_vector.begin(), coord_vector.end());

		std::queue<Eigen::Vector2i> unstable_coord;
		for(int i = 0; i != coord_vector.size(); ++i){
			unstable_coord.push(coord_vector[i]);
		}

		int iter = 0;
		while(!unstable_coord.empty()){

			// if(iter > 100000)
				// break;

			std::cout << "iter: " << iter << " size: " << unstable_coord.size() << " ";

			const Eigen::Vector2i& unstable_cell = unstable_coord.front();

			// neighbor values
			int neighbors = terrain.neighbors_info_filter(unstable_cell, values, positions, slopes, - slope_stability_threshold, false);

			proportion(neighbors, slopes, slopes_proportions);

			// stabilization
			double max_slope = max_array(neighbors, slopes);

			double available_to_transport = layers.get_field(1).at(unstable_cell);

			bool stable_after_transport = true;
			if(available_to_transport > 0.01){
				available_to_transport /= 10.;
				stable_after_transport = false;
			}

			std::cout << "neighbors: " << neighbors << " ";
			for(int i = 0; i != neighbors; ++i){
				std::cout << positions[i].x() << " " << positions[i].y() << "  ";
			}
			std::cout << std::endl;

			for(int ineigh = 0; ineigh != neighbors; ++ineigh){
				double neighbor_quantity = available_to_transport * slopes_proportions[ineigh];

				layers.get_field(1).at(positions[ineigh]) += neighbor_quantity;
				terrain.at(positions[ineigh]) += neighbor_quantity;

				// add neighbor to queue because they could be unstable now
				if(!unstable.at(positions[ineigh])){
					unstable.at(positions[ineigh]) = true;
					unstable_coord.push(positions[ineigh]);
				}

			}

			if(neighbors > 0){
				layers.get_field(1).at(unstable_cell) -= available_to_transport;
				terrain.at(unstable_cell) -= available_to_transport;
			}

			// unstable_cell becomes stable
			if(stable_after_transport){
				unstable.at(unstable_cell) = false;
				unstable_coord.pop();
			}else{
				unstable_coord.push(unstable_cell);
				unstable_coord.pop();
			}

			++iter;
		}

	}
}
