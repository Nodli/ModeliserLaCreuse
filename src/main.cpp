#include <iostream>
#include <random>
#include <string>
#include <MultiLayerMap.hpp>
#include <SimpleLayerMap.hpp>
#include <Noise/TerrainNoise.hpp>
#include <Weather/Erosion.hpp>
#include <Weather/Hydro.hpp>

void test_thermal_erosion_transport_stair(unsigned int iterations = 1){
	const int size = 40;

	// setup a test MultiLayerMap with a stair terrain
	MultiLayerMap mlm(size, size);
	SimpleLayerMap& height = mlm.new_layer();
	height = stair_layer(size, size, 1.);

	mlm.get_field(0).export_as_obj("InitialTerrain.obj");

	for(int i = 0; i != iterations; ++i){
		std::cout << "EROSION + TRANSPORT" << std::endl;
		// testing erosion without transport
		//erode_using_median_slope(mlm, 0.1);
		erode_using_mean_slope(mlm, 0.1);
		mlm.get_field(0).export_as_obj("ErodedTerrainBedrock.obj");
		mlm.get_field(1).export_as_obj("ErodedTerrainSediments.obj");
		mlm.generate_field().export_as_obj("ErodedTerrain.obj");

		// testing transport on the previously eroded terrain
		transport(mlm, 25.);
		mlm.get_field(0).export_as_obj("ErodedTransportedTerrainBedrock.obj");
		mlm.get_field(1).export_as_obj("ErodedTransportedTerrainSediments.obj");
		mlm.generate_field().export_as_obj("ErodedTransportedTerrain.obj");
	}
}

int main()
{
	//test_thermal_erosion_transport_stair(1);

	std::random_device rd;
	std::mt19937 gen(rd());

	int size = 30;
	MultiLayerMap mlm(size, size, { -5, -5}, {5, 5});
	SimpleLayerMap &sf = mlm.new_layer();
	TerrainNoise t_noise(2.5, 1.0 / 10.0, 8);

	for(int j = 0; j < size; ++j)
	{
		for(int i = 0; i < size; i++)
		{
			sf.at(i, j) = t_noise.get_noise(i, j);
		}
	}

	sf.export_as_obj("Terrain.obj");
	sf.export_as_pgm("Terrain.pgm", true);
	//SimpleLayerMap::generate_slope_map(sf).export_as_pgm("Slope.pgm", true);

	const int erosion_transport_iterations = 100;
	for(int istep = 0; istep != erosion_transport_iterations; ++istep){
		std::string folder_name = "ErosionTransport" + std::to_string(istep);
		std::string sys_cmd = "mkdir " + folder_name;
		system(sys_cmd.c_str());
		// Thermal erosion
		erode_using_median_slope(mlm, 0.01);
		mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalErosionTerrainBedrock.obj");
		mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalErosionTerrainSediments.obj");
		mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalErosionTerrain.obj");

		// Thermal transport
		transport(mlm, 20);
		mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalTransportTerrainBedrock.obj");
		mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalTransportTerrainSediments.obj");
		mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalTransportTerrain.obj");
	}

	/*
	// Hydraulic erosion, area visualization
	SimpleLayerMap area = get_area(mlm.generate_field());
	area.export_as_pgm("DistributedHydraulicArea.pgm", true);
	area = get_area(mlm.generate_field(), false);
	area.export_as_pgm("OneWayHydraulicArea.pgm", true);

	// Hydraulic erosion, terrain visualization
	SimpleLayerMap sediments = mlm.new_layer();
	sediments.set_all(0.0);

	MultiLayerMap mlmBis(mlm);
	MultiLayerMap mlmTer(mlm);
	// distributed
	erode_from_area(mlm, 0.2);
	mlm.get_field(0).export_as_pgm("TerrainDistributedHydroErode.pgm", true);
	mlm.get_field(0).export_as_obj("TerrainDistributedHydroErode.obj");
	mlm.generate_field().export_as_obj("TerrainDistributedHydroErodeAndTransport.obj");
	// one way
	erode_from_area(mlmBis, 0.2, false);
	mlmBis.get_field(0).export_as_pgm("TerrainOneWayHydroErode.pgm", true);
	mlmBis.get_field(0).export_as_obj("TerrainOneWayHydroErode.obj");
	mlmBis.generate_field().export_as_obj("TerrainOneWayHydroErodeAndTransport.obj");
	// water drop
	water_drop_transport(mlmTer, gen, 10, 1.0, 0.1, 1.0);
	mlmTer.get_field(0).export_as_pgm("TerrainWaterDropHydroErodeAndTransport.pgm", true);
	mlmTer.get_field(0).export_as_obj("TerrainWaterDropHydroErodeAndTransport.obj");
	*/

	return 0;
}

