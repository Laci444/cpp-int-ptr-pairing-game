#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <regex>
#include <utility>

auto createGenerator(const std::mt19937 &mt, int lower_bound, int upper_bound) {
  std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
  return std::bind(dist, mt);
}

int getUserInput() {
  std::cout << "Add meg, mennyi int-pointer part szeretnel! ";
  int numberOfPairs;
  std::cin >> numberOfPairs;

  while (!std::cin) {
    std::cout << "Egy pozitiv egesz szamot adj meg! ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin >> numberOfPairs;
  }

  return numberOfPairs;
}

void writeToScreen(int *numbers, int **pointers, int length) {
  std::cout << "Ezek a szamok maradtak: " << std::endl;
  for (int i = 0; i < length; i++) {
    if (numbers[i] == 0)
      continue;
    std::cout << i + 1 << ": " << numbers[i] << std::endl;
  }
  std::cout << "Ezek pedig a pointerek:" << std::endl;
  for (int i = 0; i < length; i++) {
    if (pointers[i] == NULL)
      continue;
    std::cout << i + 1 << ": " << pointers[i] << std::endl;
  }
  std::cout << "Tippelj egy part (szam:szam): ";
}

std::pair<int, int> getGuess(int maxGuess) {
  std::string input;
  std::regex reg("^[0-9]*:[0-9]*$");
  int colonIndex, numberGuess, pointerGuess;
  while (true) {
    std::cin >> input;

    if (!std::cin) {
      std::cout << "A szintaxis: szam:szam! ";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    if (!std::regex_match(input, reg)) {
      std::cout << "A szintaxis: szam:szam! ";
      continue;
    }

    colonIndex = input.find_first_of(':');
    numberGuess = std::stoi(input.substr(0, colonIndex));
    pointerGuess = std::stoi(input.substr(colonIndex + 1, input.size()));

    if (numberGuess > maxGuess || pointerGuess > maxGuess) {
      std::cout << "A legnagyobb lehetseges bemenet " << maxGuess
                << ". Probald ujra! " << std::endl;
      continue;
    }
    return std::pair(numberGuess, pointerGuess);
  }
}

int main() {
  std::random_device rd;
  std::mt19937 mt(rd());
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

  std::shuffle(pointers, pointers + numberOfPairs, mt);

  int counter = 0;
  // game loop
  while (true) {
    writeToScreen(numbers, pointers, numberOfPairs);

    std::pair<int, int> guess = getGuess(numberOfPairs);

    std::cout << "A tipped (" << guess.first << " : " << guess.second << ") ";

    if (&numbers[guess.first - 1] == pointers[guess.second - 1]) {
      std::cout << "HELYES!!" << std::endl;
      numbers[guess.first - 1] = 0;
      pointers[guess.second - 1] = NULL;
      counter++;
    } else {
      std::cout << "sajnos helytelen :(" << std::endl;
    }
    if (counter >= numberOfPairs)
      break;
  }

  std::cout << "Gratulalok! Sikeresen eltalaltad az osszes part!" << std::endl;

  return 0;
}
