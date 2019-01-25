#include <Vegetation/Vegetation.hpp>

#include <iostream>

void generate_distribution(const MultiLayerMap& m)
{
	BiomeInfo bi(m);
	int maxVal = 255;
	SimpleLayerMap distrib(static_cast<Grid2d>(m));
	distrib.set_all(0.0);
	MultiLayerMap distribs(static_cast<Grid2d>(m));
	distribs.add_field(low_grass_density(bi));
	distribs.add_field(bush_density(bi));
	distribs.add_field(tree_density(bi));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, m.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, m.grid_width() - 1);
	std::uniform_real_distribution<> rdis(0, 1);
	bool nope = true;

	for(int i = 0; i < 15000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(0).value(x, y))
			{
				distrib.set_value(x, y, 1.0);
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < 4000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(1).value(x, y))
			{
				distrib.set_value(x, y, 2.0);
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < 4000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(2).value(x, y))
			{
				distrib.set_value(x, y, 3.0);
				nope = false;
			}
		}
		while(nope);
	}

	distrib.export_as_pgm("TEST.pgm", true);
	std::string filename = "distribT.ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distrib.grid_width() << " " << distrib.grid_height() << std::endl;
	output << maxVal << std::endl;

	for(int j = distrib.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distrib.grid_width(); ++i)
		{
			if(distrib.value(i, j) == 0)
			{
				output << 0 << " " << 0 << " " << 0 << " ";
			}
			else if(distrib.value(i, j) == 1)
			{
				output << 30 << " " << 255 << " " << 10 << " ";
			}
			else if(distrib.value(i, j) == 2)
			{
				output << 10 << " " << 15 << " " << 200 << " ";
			}
			else if(distrib.value(i, j) == 3)
			{
				output << 255 << " " << 100 << " " << 100 << " ";
			}
		}

		output << std::endl;
	}

	output.close();
}

void save_simulation(VegetationLayerMap& distribution, int iter)
{
	std::string filename = "Simu_6/simulation_" + std::to_string(iter / 10) + ".ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distribution.grid_width() << " " << distribution.grid_height() << std::endl;
	output << 255 << std::endl;

	for(int j = distribution.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distribution.grid_width(); ++i)
		{
			int size = distribution.at(i, j).size();
			if(size != 0){
				int nb_h_grass = distribution.count_ID_at(i, j, 0);
				double force_h_grass = (double)nb_h_grass/10;
				double prop_h_grass = force_h_grass*(double)nb_h_grass/size;
				int nb_bush =    distribution.count_ID_at(i, j, 1);
				double force_bush = (double)nb_bush/10;
				double prop_bush = force_bush*(double)nb_bush/size;
				int nb_tree =    distribution.count_ID_at(i, j, 2);
				double force_tree = (double)nb_tree/10;
				double prop_tree = force_tree*(double)nb_tree/size;
				int nb_grass1 =  distribution.count_ID_at(i, j, 3);
				double force_grass1 = (double)nb_grass1/10;
				double prop_grass1 = force_grass1*(double)nb_grass1/size;
				int nb_grass2 =  distribution.count_ID_at(i, j, 4);
				double force_grass2 = (double)nb_grass2/10;
				double prop_grass2 = force_grass2*(double)nb_grass2/size;
				int r = 76  * prop_h_grass + 236 * prop_bush + 8   * prop_tree + 165 * prop_grass1 + 202 * prop_grass2;
				int g = 119 * prop_h_grass + 174 * prop_bush + 78  * prop_tree + 208 * prop_grass1 + 178 * prop_grass2;
				int b = 38  * prop_h_grass + 93  * prop_bush + 0   * prop_tree + 24  * prop_grass1 + 21  * prop_grass2;
				output << r<< " " << g<< " " <<	b<< " ";
			}
			else
			{
				output << 0 << " " << 0 << " " << 0 << " ";
			}
		}

		//std::cout << std::endl;
		output << std::endl;
	}

	output.close();
}

void simulate(const MultiLayerMap& mlm)
{
	BiomeInfo bi(mlm);
	VegetationLayerMap distribution(static_cast<Grid2d>(mlm));
	SimpleLayerMap g_density = strong_grass_density(bi);
	SimpleLayerMap g_density2 = low_grass_density(bi);
	SimpleLayerMap b_density = bush_density(bi);
	SimpleLayerMap t_density = tree_density(bi);
	g_density.export_as_pgm("DensityGrass.pgm");
	g_density2.export_as_pgm("DensityGrass2.pgm");
	b_density.export_as_pgm("DensityBush.pgm");
	t_density.export_as_pgm("DensityTree.pgm", false, 0, 1);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, mlm.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, mlm.grid_width() - 1);
	std::uniform_real_distribution<> rdis(0, 1);
	bool nope = true;
	Grass ref_grass(0, 30, 5, 1.0, &g_density);
	Grass ref_grass2(3, 25, 1, 1.0, &g_density2);
	Grass ref_grass3(4, 25, 1, 1.0, &g_density2);
	Bush ref_bush(1, 100, 30, 1.0, &b_density);
	Tree ref_tree(2, 200, 50, 1.0, &t_density);
	int nb_seeds = 500;

	for(int i = 0; i < nb_seeds; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < g_density.value(x, y))
			{
				distribution.at(x, y).push_back(new Grass(ref_grass));
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < nb_seeds; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < g_density2.value(x, y))
			{
				distribution.at(x, y).push_back(new Grass(ref_grass2));
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < nb_seeds; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < g_density2.value(x, y))
			{
				distribution.at(x, y).push_back(new Grass(ref_grass3));
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < nb_seeds; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < b_density.value(x, y))
			{
				distribution.at(x, y).push_back(new Bush(ref_bush));
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < nb_seeds/2; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < t_density.value(x, y))
			{
				distribution.at(x, y).push_back(new Tree(ref_tree));
				nope = false;
			}
		}
		while(nope);
	}

	save_simulation(distribution, 0);

	for(int it = 1; it <= 5000; ++it)
	{
		for(int j = 0; j < distribution.grid_height(); ++j)
		{
			for(int i = 0; i < distribution.grid_width(); i++)
			{
				std::vector<Plant*>& cell = distribution.at(i, j);
				auto p = cell.begin();

				while(p != cell.end())
				{
					(*p)->update(gen, rdis, distribution, i, j);

					if((*p)->is_dead())
					{
						delete *p;
						p = cell.erase(p);
					}
					else
					{
						p++;
					}
				}
			}
		}

		if(it % 10 == 0)
		{
			save_simulation(distribution, it);
		}
	}
}