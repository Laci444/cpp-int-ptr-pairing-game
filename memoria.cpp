#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <random>
#include <utility>

using namespace std;

auto createGenerator(mt19937 &mt, int lower_bound, int upper_bound) {
  uniform_int_distribution<int> dist(lower_bound, upper_bound);
  return bind(dist, mt);
}

int getUserInput() {
  cout << "Add meg, mennyi int-pointer part szeretnel! ";
  int numberOfPairs;
  cin >> numberOfPairs;

  while (!cin) {
    cout << "Egy pozitiv egesz szamot adj meg! ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> numberOfPairs;
  }

  return numberOfPairs;
}

void writeToScreen(int *numbers, int **pointers, int length) {
  cout << "Ezek a szamok maradtak: " << endl;
  for (int i = 0; i < length; i++) {
    if (numbers[i] == 0)
      continue;
    cout << i + 1 << ": " << numbers[i] << endl;
  }
  cout << "Ezek pedig a pointerek:" << endl;
  for (int i = 0; i < length; i++) {
    if (pointers[i] == NULL)
      continue;
    cout << i + 1 << ": " << pointers[i] << endl;
  }
  cout << "Tippelj egy part (szam:szam): ";
}

pair<int, int> getGuess() {
  string input;
  cin >> input;

  while (!cin) {
    cout << "A szintaxis: szam:szam! ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin >> input;
  }

  // oh god no i wont validate this input
  // after half an our googling i dont want to do this
  // lets just pretend every input is perfect

  int colonIndex = input.find_first_of(':');
  int numberGess = stoi(input.substr(0, colonIndex));
  int pointerGess = stoi(input.substr(colonIndex + 1, input.size()));

  return pair(numberGess, pointerGess);
}

int main() {
  random_device rd;
  mt19937 mt(rd());
  // create pseudo random number generator that yields from 1 to 100
  auto generator = createGenerator(mt, 1, 100);
  // get user imput on how many number-pointer pairs they want to generate
  int numberOfPairs = getUserInput();
  // generate array of random values and their pointers
  int numbers[numberOfPairs];
  int *pointers[numberOfPairs];
  for (int i = 0; i < numberOfPairs; i++) {
    numbers[i] = generator();
    pointers[i] = &numbers[i];
  }

  shuffle(pointers, pointers + numberOfPairs, mt);

  int counter = 0;
  // game loop
  while (true) {
    writeToScreen(numbers, pointers, numberOfPairs);

    pair<int, int> guess = getGuess();

    cout << "A tipped (" << guess.first << " : " << guess.second << ") ";

    if (&numbers[guess.first - 1] == pointers[guess.second - 1]) {
      cout << "HELYES!!" << endl;
      numbers[guess.first - 1] = 0;
      pointers[guess.second - 1] = NULL;
      counter++;
    } else {
      cout << "sajnos helytelen :(" << endl;
    }
    if (counter >= numberOfPairs)
      break;
  }

  cout << "Gratulalok! Sikeresen eltalaltad az osszes part!" << endl;

  return 0;
}
