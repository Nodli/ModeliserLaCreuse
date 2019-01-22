#include <Weather/Biome.hpp>

BiomeInfo::BiomeInfo(const MultiLayerMap& m)
	: slope(SimpleLayerMap::generate_slope_map(m).normalize())
	, exposure(get_light_exposure(m).normalize())
	, water_index(get_water_indexes(m).normalize())
	, height(m.generate_field().normalize())
	, sediments(m.get_field(1)) 
	{
		sediments.normalize();
	}

SimpleLayerMap get_light_exposure(const DoubleField& df, const int nb_steps, const int nb_samples)
{
	SimpleLayerMap res(static_cast<Grid2d>(df));
	double total = nb_samples * 3.1415 / 2.0;

	for(int j = 0; j < res.grid_height(); ++j)
	{
		for(int i = 0; i < res.grid_width(); ++i)
		{
			double val = df.value_safe(i, j);
			double sum_exp = 0;

			for(int d = 0; d < nb_samples; d++)
			{
				double angle = (2.*3.1415) * d / nb_samples;
				Eigen::Vector2d delta_pos = {cos(angle), sin(angle)};
				double covA = 0;
				double h = 0;

				for(int s = 0; s < nb_steps; ++s)
				{
					double v = df.value_safe(i + s * delta_pos(0), j + s * delta_pos(1)) - val;

					if(v > 0)
					{
						double tmpCov = atan(v / delta_pos.norm());

						if(tmpCov > covA)
						{
							covA = tmpCov;
						}
					}
				}

				sum_exp += (3.1415 / 2.0) - covA;
			}

			res.at(i, j) = sum_exp / total;
		}
	}

	return res;
}
