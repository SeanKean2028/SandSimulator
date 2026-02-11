#include "Random.h"
Random& Random::getInstance() {
	static Random instance;
	return instance;
}
void Random::Reseed() {
	generator.seed(randomDevice());
}
void Random::ShowMessage() {
	cout << "----- Random Singelton -----\n";
	cout << "Seed: " << generator << "\n";
}
int Random::GetRandomInt(int _min, int _max) {
	uniform_int_distribution<> dist(_min, _max);
	return(dist(generator));
}
Random::Random() {
	mt19937 gen(randomDevice());
	generator.seed(randomDevice());
}