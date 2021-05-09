#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <time.h>

#define winWidth 1000.0
#define winHeight 500.0
#define optionCount 4

using namespace std;
class bar :public sf::RectangleShape {
public:
	int value;
	bar(int v = 0) {
		value = v;
		setFillColor(sf::Color::Green);
		//setOutlineColor(sf::Color::Black);
		//setOutlineThickness(3);
	}
	void setX(float x) {
		sf::Vector2f pos = getPosition();
		pos.x = x;
		setPosition(pos);
	}
};

void swap(bar*& a, bar*& b) {
	bar* temp = a;
	a = b;
	b = temp;
}

void color(float* rgb, float offset) {
	static int c = 1;
	static bool d = true;
	if (!d)
		offset = -offset;

	rgb[c] = rgb[c] + offset;

	for (int i = 0; i < 3; i++) {
		if (i != c) {
			if (rgb[c] == rgb[i]) {
				d = !d;
				c = i;
				break;
			}
		}
	}
}
void drawBars(vector<bar*>& bars, sf::RenderWindow& window) {
	for (int i = 0; i < bars.size(); i++) {
		float x = i * (winWidth / bars.size());
		bars[i]->setX(x);
		window.draw(*bars[i]);
	}
}
void initialize(vector<bar*>& bars) {
	int n = bars.size();

	float rgb[3] = { 10 ,130, 250 };
	int offsets[16] = { 1,2,4,5,8,10,20,25,40,50,100,125,200,250,500,1000 };
	float offset = bars.size();
	for (int i = 0; i < 16; i++) {
		if (offsets[i] <= bars.size()) {
			offset = offsets[i];
		}
	}
	offset = 1000.0 / offset;

	for (int i = 0; i < n; i++) {
		float height = ((i + 1.0) / n) * winHeight;
		float width = winWidth / n;
		float y = winHeight - height;

		bars[i] = new bar;
		bars[i]->value = i + 1;
		bars[i]->setSize(sf::Vector2f(width, height));
		bars[i]->setPosition(sf::Vector2f(0, y));

		color(rgb, offset);
		bars[i]->setFillColor(sf::Color(rgb[0], rgb[1], rgb[2]));
	}
}
void shuffle(vector<bar*>& bars, sf::Text text, sf::RenderWindow& window, sf::Event event) {
	text.setString("Shuffling");
	srand(time(nullptr));
	int end = bars.size() - 1;
	while (end != 0) {
		int rindex = rand() % (end + 1);

		bar* temp = bars[end];
		bars[end] = bars[rindex];
		bars[rindex] = temp;


		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				text.setString("Shuffle");
				return;
			}
		}
		window.clear();
		drawBars(bars, window);
		window.draw(text);
		window.display();
		end--;
	}
	text.setString("Shuffle");
}

int selectMin(const vector<bar*>& bars, int start) {
	int min = start;
	for (int i = start + 1; i < bars.size(); i++) {
		if (bars[i]->value < bars[min]->value)
			min = i;
	}
	return min;
}
void selectionSort(vector<bar*>& bars, sf::Text& text, sf::RenderWindow& window, sf::Event event) {
	text.setString("Selection Sort - Running");
	for (int i = 0; i < bars.size() - 1; i++) {
		int min = selectMin(bars, i);
		bar* temp = bars[i];
		bars[i] = bars[min];
		bars[min] = temp;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}
			if (event.type == sf::Event::KeyPressed) {
				text.setString("Selection Sort");
				return;
			}
		}
		window.clear();
		drawBars(bars, window);
		window.draw(text);
		window.display();
	}
	text.setString("Selection Sort");
}

void bubbleSort(vector<bar*>& bars, sf::Text& text, sf::RenderWindow& window, sf::Event& event) {
	text.setString("Bubble Sort - Running");
	int n = bars.size();
	for (int i = 0; i < n - 1; i++) {
		bool noSwaps = true;
		for (int j = 0; j < n - i - 1; j++) {
			if (bars[j]->value > bars[j + 1]->value) {
				swap(bars[j], bars[j + 1]);
				noSwaps = false;
			}

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
					return;
				}
				if (event.type == sf::Event::KeyPressed) {
					text.setString("Bubble Sort");
					return;
				}
			}

			window.clear();
			drawBars(bars, window);
			window.draw(text);
			window.display();
		}
		if (noSwaps)
			break;
	}
	text.setString("Bubble Sort");
}

void merge(vector<bar*>& bars, int s1, int e1, int s2, int e2, sf::RenderWindow& window) {
	vector<bar*> barsCopy = bars;
	for (int i = s1; i <= e2; i++) {
		if (s1 > e1)
			bars[i] = barsCopy[s2++];
		else if (s2 > e2)
			bars[i] = barsCopy[s1++];
		else if (barsCopy[s2]->value < barsCopy[s1]->value) {
			bars[i] = barsCopy[s2++];
		}
		else {
			bars[i] = barsCopy[s1++];
		}
	}
}
void mergeSort(vector<bar*>& bars, sf::Text& text, sf::RenderWindow& window, sf::Event event) {
	text.setString("Merge Sort - Running");
	for (int subLength = 1; subLength < bars.size(); subLength *= 2) {
		for (int s1 = 0; s1 < bars.size(); s1 += 2 * subLength) {
			int e1 = s1 + subLength - 1;
			int s2 = e1 + 1;
			int e2 = s1 + 2 * subLength - 1;

			if (e1 >= bars.size())
				e1 = bars.size() - 1;
			if (e2 >= bars.size())
				e2 = bars.size() - 1;

			merge(bars, s1, e1, s2, e2, window);

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed) {
					window.close();
					return;
				}
				if (event.type == sf::Event::KeyPressed) {
					text.setString("Merge Sort");
					return;
				}
			}
			window.clear();
			drawBars(bars, window);
			window.draw(text);
			window.display();
		}
	}
	text.setString("Merge Sort");
}

int main(int argc, char** argv) {
	int n = 100;
	if (argc > 1)
		n = atoi(argv[1]);

	vector<bar*> bars(n);
	initialize(bars);
	int option = 0;

	sf::RenderWindow window(sf::VideoMode(winWidth, winHeight), "MergeSort", sf::Style::Default);

	sf::Text text;
	text.setCharacterSize(15);
	text.setPosition(sf::Vector2f(0, 0));
	text.setFillColor(sf::Color::Green);
	sf::Font consola;
	consola.loadFromFile("consola.ttf");
	text.setFont(consola);
	text.setString("Shuffle");

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				else if (event.key.code == sf::Keyboard::Space) {
					switch (option) {
					case 0:
						shuffle(bars, text, window, event);
						break;
					case 1:
						selectionSort(bars, text, window, event);
						break;
					case 2:
						window.setFramerateLimit(0);
						bubbleSort(bars, text, window, event);
						break;
					case 3:
						mergeSort(bars, text, window, event);
						break;
					default:
						break;
					}
				}
				else if (event.key.code == sf::Keyboard::Left) {
					option = (option - 1);
					if (option < 0)
						option += optionCount;
				}
				else if (event.key.code == sf::Keyboard::Right) {
					option = (option + 1) % optionCount;
				}
				switch (option) {
				case 0:
					text.setString("Shuffle");
					break;
				case 1:
					text.setString("Selection Sort");
					break;
				case 2:
					text.setString("Bubble Sort");
					break;
				case 3:
					text.setString("Merge Sort");
					break;
				default:
					break;
				}
			}
		}
		if (n < 500)
			window.setFramerateLimit(0.3 * n);
		else
			window.setFramerateLimit(0);


		window.clear();

		drawBars(bars, window);
		window.draw(text);

		window.display();
	}
	return 0;
}