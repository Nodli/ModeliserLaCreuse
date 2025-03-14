#include "Noise/TerrainNoise.hpp"

//FastNoise noise;
//noise.SetNoiseType(FastNoise::Perlin);

double TerrainNoise::get_noise(int i, int j)
{
	double value = 0.0;
	double freq = _base_freq;
	double ampl = _amplitude;
	for(int k = 0; k < _octaves; k++)
	{
		_base_noise.SetFrequency(freq);
		_ridge_noise.SetFrequency(freq);
		freq *= 2.0;
		double tv = _base_noise.GetNoise(i + k * 100, j + k * 100);
		double rv = _ridge_noise.GetNoise(i + k * 100, j + k * 100);
		double v = (tv < rv) ? tv : 2 * rv - tv;
		double kf = 1.0 - 1.0 / (double)(0.5 + k * k);

		if(k > 0)
		{
			value += (1 - (1 - ((value + _amplitude) / (2.0*_amplitude))) * kf) * ampl * v;
			//sf.at(i, j) += ((sf.at(i, j) + 5) / 10.0) * ampl * v;
		}
		else
		{
			value += ampl * v;
		}

		ampl /= 2.0;
	}

	return (value+2*_amplitude);//*noi;
}

double TerrainNoise::get_noise2(int i, int j)
{
	_biome_noise.SetFrequency(_base_freq/2.0);
	double noi = (0.5 + 0.5*_biome_noise.GetNoise(i, j));

	double val = get_noise(i, j);

	return val*noi;
}

double TerrainNoise::get_noise3(int i, int j)
{
	_biome_noise.SetFrequency(_base_freq/4.0);
	//int oct = _octaves;
	double noi = (0.5 + 0.5*_biome_noise.GetNoise(i, j));
	//_octaves = noi * _octaves;

	double val = get_noise(i, j);

	//_octaves = oct;
	return val*noi;
}

SimpleLayerMap stair_layer(int width, int height, double amplitude){

	SimpleLayerMap stair(width, height, {0, 0}, {1, 1});

	for(int h = 0; h < height; ++h){
		for(int w = 0; w < width; ++w){
			if(w < (width / 2)){
				stair.set_value(w, h, amplitude / 2.);
			}else{
				stair.set_value(w, h, - amplitude / 2.);
			}
		}
	}

	return stair;
}

SimpleLayerMap double_stair_layer(int width, int height, double amplitude){

	SimpleLayerMap double_stair(width, height, {0, 0}, {1, 1});

	for(int h = 0; h < height; ++h){
		for(int w = 0; w < width; ++w){
			if((w < width / 2) && (h < height / 2)){
				double_stair.set_value(w, h, amplitude / 2.);
			}else if((w < width / 2)){
				double_stair.set_value(w, h, 4. / 3. * amplitude - amplitude); // (2 * amplitude) * 2 / 3 - amplitude
			}else if((h < height / 2)){
				double_stair.set_value(w, h, - amplitude);
			}else{
				double_stair.set_value(w, h, 2. / 3. * amplitude - amplitude); // (2 * amplitude) * 1 / 3 - amplitude
			}
		}
	}

	return double_stair;
}
