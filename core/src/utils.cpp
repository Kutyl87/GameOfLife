#include <array>
#include <cmath>
#include <random>

float generateRandomNumber(float min, float max) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(min, max);
	return dis(gen);
}

float calculateDistance(const std::array<float, 3>& point1, const std::array<float, 3>& point2) {
	float x_diff = point1[0] - point2[0];
	float y_diff = point1[1] - point2[1];
	float z_diff = point1[2] - point2[2];
	return std::sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}

std::array<float, 3> calculateMidpoint(const std::array<float, 3>& point1, const std::array<float, 3>& point2) {
	return {(point1[0] + point2[0]) / 2, (point1[1] + point2[1]) / 2, (point1[2] + point2[2]) / 2};
}