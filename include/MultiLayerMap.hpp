#pragma once

#include <ScalarField.hpp>

/**
 * @brief Defines a layered field
 *
 */
class MultiLayerMap : public Grid2d
{
public:
	MultiLayerMap() = delete;
	/**
	 * @brief Construct a new Multi Layer Map object from an other one
	 *
	 * @param map       the Multi Layer Map to copy
	 */
	MultiLayerMap(const MultiLayerMap& map) : Grid2d(map), _fields(map._fields) {}
	/**
	 * @brief Construct a new Multi Layer Map object from an other one
	 *
	 * @param map       the Multi Layer Map to copy
	 */
	MultiLayerMap(MultiLayerMap&& map) : Grid2d(std::move(map)), _fields(std::move(map._fields)) {}
	/**
	 * @brief Construct a new Multi Layer Map object from scratch
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	MultiLayerMap(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: Grid2d(width, height, a, b) {}

	/**
	 * @brief Get the number of layers
	 *
	 * @return int      the number of layers
	 */
	int get_layer_number() const
	{
		return _fields.size();
	}

	/**
	 * @brief Get the a field of the Multi Layer Map
	 *
	 * @param field_index           the index of the field in the map
	 * @return const ScalarField&   a reference to the field
	 */
	const ScalarField& get_field(const int field_index) const
	{
		return _fields.at(field_index);
	}
	/**
	 * @brief Get the a field of the Multi Layer Map
	 *
	 * @param field_index           the index of the field in the map
	 * @return const ScalarField&   a modifiable reference to the field
	 */
	ScalarField& get_field(const int field_index)
	{
		return _fields.at(field_index);
	}

	/**
	 * @brief Generate an agregation of the Multi Layer Map.
	 * This is done by summing all the Scalar Fields together
	 *
	 * @return ScalarField      the Scalar Field sum of all the field in the Multi Layer Map
	 */
	ScalarField generate_field() const;

	/**
	 * @brief Set the value of a field at a given position
	 *
	 * @param field_index       the index of the field to modify
	 * @param i, j              the position of the cell to modify
	 * @param v                 the new value to set
	 */
	void set_value(const int field_index, const int i, const int j, const double v)
	{
		_fields.at(field_index).set_value(i, j, v);
	}

	/**
	 * @brief Set the values of a whole field
	 *
	 * @param field_index       the index of the field to modify
	 * @param field             the field from which to copy the values
	 */
	void set_field(int field_index, const ScalarField& field)
	{
		while((field_index + 1) > _fields.size()){
			new_field();
		}

		_fields.at(field_index).copy_values(field);
	}
	/**
	 * @brief Set the values of a whole field
	 *
	 * @param field_index       the index of the field to modify
	 * @param field             the field from which to copy the values
	 */
	void set_field(int field_index, ScalarField&& field)
	{
		_fields.at(field_index).copy_values(std::move(field));
	}

	/**
	 * @brief Add a field to the Multi Layer Map
	 *
	 * @param field             the field to copy into the Multi Layer Map
	 */
	void add_field(const ScalarField& field)
	{
		_fields.push_back(field);
	}
	/**
	 * @brief Add a field to the Multi Layer Map
	 *
	 * @param field             the field to copy into the Multi Layer Map
	 */
	void add_field(ScalarField&& field)
	{
		_fields.push_back(std::move(field));
	}

	/**
	 * @brief Add a new layer to the map
	 *
	 * @return ScalarField&     a reference to the newly created layer
	 */
	ScalarField& new_field();

	/**
	 * @brief Reshape the MultiLayerMap to a new size and position
	 *
	 * @param ax first x position
	 * @param ay first y position
	 * @param bx second x position
	 * @param by second y position
	 */
	void reshape(const double ax, const double ay, const double bx, const double by);
	/**
	 * @brief Reshape the MultiLayerMap to a new size and position
	 *
	 * @param a first point of the box
	 * @param b second point of the box
	 */
	void reshape(const Eigen::Vector2d a, const Eigen::Vector2d b);

	/**
	 * @brief Affectation operator
	 *
	 * @param mlm               the Multi Layer Map to affect
	 * @return MultiLayerMap&   a reference to this Multi Layer Map
	 */
	MultiLayerMap& operator=(const MultiLayerMap& mlm);
	/**
	 * @brief Affectation operator
	 *
	 * @param mlm               the Multi Layer Map to affect
	 * @return MultiLayerMap&   a reference to this Multi Layer Map
	 */
	MultiLayerMap& operator=(MultiLayerMap&& mlm);

protected:
	std::vector<ScalarField> _fields; /**< Array of Scalar Fields*/
};