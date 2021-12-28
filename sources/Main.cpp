#include <SFML/Graphics.hpp>
#include <locale.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>

#include "head_file.h"


class Get_Coefficient
{
private:
	std::string m_func;
	std::vector<std::vector<std::string>> m_polynomial;
	int m_current_symbol = 0;
	
	std::vector<int> split(std::string str)
	{
		std::vector<std::string> arr;

		std::string delim(" ");
		size_t prev = 0;
		size_t next;
		size_t delta = delim.length();

		while ((next = str.find(delim, prev)) != std::string::npos) {
			//Отладка-start
			std::string tmp = str.substr(prev, next - prev);
			//Отладка-end
			arr.push_back(str.substr(prev, next - prev));
			prev = next + delta;
		}
		//Отладка-start
		std::string tmp = str.substr(prev);
		//Отладка-end
		arr.push_back(str.substr(prev));

		std::vector<int> int_arr;
		for (int i = 0; i < arr.size(); i++)
		{
			int_arr[i] = std::stoi(arr[i]);
		}
		return int_arr;
	}

	void separate_by_operator()
	{
		size_t len = m_func.length();
		int member_count = 0;
		std::string member;
		std::vector<std::vector<std::string>> polynomial;
		for (int i = 1; i < len;)
		{
			member = "";
			polynomial.resize(member_count + 1);
			if (m_func[i] == '+')
			{
				polynomial[member_count].push_back("+");
				for (i++; i < len; i++)
				{
					if (m_func[i] != '+' && (m_func[i] != '-' || m_func[i - 1] == '*' || m_func[i - 1] == '/')) member += m_func[i];
					else break;
				}
				polynomial[member_count].push_back(member);
				member_count++;
			}
			else if (m_func[i] == '-')
			{
				polynomial[member_count].push_back("-");
				for (i++; i < len; i++)
				{
					if (m_func[i] != '+' && (m_func[i] != '-' || m_func[i - 1] == '*' || m_func[i - 1] == '/')) member += m_func[i];
					else break;
				}
				polynomial[member_count].push_back(member);
				member_count++;
			}

		}
		m_polynomial = polynomial;
	}

	bool is_number(char symbol)
	{
		if (symbol == '0' || symbol == '1' || symbol == '2' || symbol == '3'
			|| symbol == '4' || symbol == '5' || symbol == '6' || symbol == '7'
			|| symbol == '8' || symbol == '9' || symbol == '.' || symbol == '-')
			return true;
		else
			return false;
	}

	std::vector<std::vector<std::string>> separate_by_operator_mu_di(std::string& const member)
	{
		size_t len = member.length();
		int member_count = 0;
		std::string multiplier;
		std::vector<std::vector<std::string>> multipliers;
		for (int i = 0; i < len;)
		{
			multiplier = "";
			multipliers.resize(member_count + 1);
			if (member[i] == '*')
			{
				multipliers[member_count].push_back("*");
				for (i++; i < len; i++)
				{
					if (member[i] != '*' && member[i] != '/') multiplier += member[i];
					else break;
				}
				multipliers[member_count].push_back(multiplier);
				member_count++;
			}
			else if (member[i] == '/')
			{
				multipliers[member_count].push_back("/");
				for (i++; i < len; i++)
				{
					if (member[i] != '*' && member[i] != '/') multiplier += member[i];
					else break;
				}
				multipliers[member_count].push_back(multiplier);
				member_count++;
			}

		}
		return multipliers;
	}

	double get_number(std::string& const member, int k, bool left = false)
	{
		std::string number = "";
		size_t len = member.length();
		if (left)
		{
			for (int i = 0; i < k; i++)
			{
				if (is_number(member.at(i))) number += member.at(i);
				else break;
			}
		}
		else
		{
			for (int i = k + 1; i < len; i++)
			{
				if (is_number(member.at(i))) number += member.at(i);
				else break;
			}
		}
		return std::stod(number);
	}

	void x_processing(double const x, int i, std::string& member)
	{
		// i - индекс x
		int degree = 1;
		if (member[i + 1] == '^')
		{
			degree = get_number(member, i + 1);
			member.erase(i, i + 2);
		}
		else member.erase(i, 1);

		member.insert(i, ((i == 1 || member[i - 1] == '/' || member[i - 1] == '*') ? "" : "*") + std::to_string(pow(x, degree)));
	}

	double member_processing(double const x, std::string& member)
	{
		double num = 1;
		double degree = 1;
		member += "*1";
		member.insert(0, "*");
		for (int i = 0; i <= member.length(); i++)
		{
			if (member[i] == 'x')
			{
				x_processing(x, i, member);
				i = 0;
			}
		}
		std::vector<std::vector<std::string>> multipliers = separate_by_operator_mu_di(member);

		for (int i = 0; i < multipliers.size(); i++)
		{
			if (multipliers[i][0] == "*") num *= std::stod(multipliers[i][1]);
			if (multipliers[i][0] == "/") num /= std::stod(multipliers[i][1]);
		}
		return num;
	}

public:
	Get_Coefficient(std::string func) 
		: m_func(func)
	{}

	//удаляет пробелы
	void func_split()
	{
		size_t len = m_func.length();
		std::string tmp = "+";
		for (int i = 0; i < len; ++i)
		{
			if (m_func.at(i) != ' ') tmp += m_func.at(i);
		}
		if (tmp.at(1) == '-') tmp.at(0) = ' ';
		m_func = tmp.insert(0, " ");
	}

	int get_y_delta(int max_y)
	{
		int delta = 0;

		return delta;
	}

	double get_y(double const x)
	{
		separate_by_operator();
		double y = 0;
		double num = 0;
		size_t len = m_func.length();
		

		for (int i = 0; i < m_polynomial.size(); i++)
		{
			if (m_polynomial[i][0] == "+") y += member_processing(x, m_polynomial[i][1]);
			else if (m_polynomial[i][0] == "-") y -= member_processing(x, m_polynomial[i][1]);
		}
		return y;
	}
};


//Класс с методами постороения графиков и оси координат
class Graphics
{
private:
	sf::RenderWindow& m_window;
	sf::Font m_font;
	Get_Coefficient m_gc;

public:
	Graphics(sf::RenderWindow& window, sf::Font font, Get_Coefficient& gc) : m_window(window), m_gc(gc), m_font(font)
	{}

	//Создание оси координат
	void coordinate_axes()
	{
		//Ось x
		sf::Vertex x_axis[] =
		{
			sf::Vertex(sf::Vector2f(0.f, 400.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(800.f, 400.f), sf::Color::Black)
		};
		m_window.draw(x_axis, 2, sf::Lines);
		
		for (int i = 2; i < 40; i++)
		{
			sf::Vertex x_axis_line[] =
			{
				sf::Vertex(sf::Vector2f(20.0 * i, 0), sf::Color(120, 120, 120, 90)),
				sf::Vertex(sf::Vector2f(20.0 * i, 782), sf::Color(120, 120, 120, 90))
			};


			sf::Text line_number("", m_font, 13);

			if (i < 20)
			{
				line_number.setString(std::to_string(-20 + i));
				line_number.setPosition(20.0 * i - std::to_string(-20 + i).length() / 2 - 8, 780);
			}
			else if (i > 20)
			{
				line_number.setString(std::to_string(i - 20));
				line_number.setPosition(20.0 * i - std::to_string(i - 20).length() / 2 - 8, 780);
			}

			line_number.setFillColor(sf::Color::Black);

			m_window.draw(line_number);
			m_window.draw(x_axis_line, 2, sf::Lines);
		}

		//Ось y
		sf::Vertex y_axis[] =
		{
			sf::Vertex(sf::Vector2f(400.0, 0.f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(400.0, 800.f), sf::Color::Black)
		};
		m_window.draw(y_axis, 2, sf::Lines);

		for (int i = 1; i < 40; i++)
		{
			sf::Vertex y_axis_line[] =
			{
				sf::Vertex(sf::Vector2f(30, 20.0 * i), sf::Color(120, 120, 120, 90)),
				sf::Vertex(sf::Vector2f(790, 20.0 * i), sf::Color(120, 120, 120, 90))
			};

			sf::Text line_number("", m_font, 13);
			if (i < 20)
			{
				line_number.setString(std::to_string(-(i - 20)));
				line_number.setPosition(8, -10 + 20.0 * i);
			
			}
			else if (i > 20)
			{
				line_number.setString(std::to_string(-(i - 20)));
				line_number.setPosition(2, -10 + 20.0 * i);
			}
			line_number.setFillColor(sf::Color::Black);

			m_window.draw(line_number);
			m_window.draw(y_axis_line, 2, sf::Lines);
		}

		sf::Text line_number("0", m_font, 13);
		line_number.setPosition(390, 400);
		line_number.setFillColor(sf::Color::Black);
		m_window.draw(line_number);

		sf::Text axis("y", m_font, 13);
		axis.setFillColor(sf::Color::Black);
		axis.setPosition(425, 0);
		m_window.draw(axis);

		axis.setString("x");
		axis.setPosition(790, 415);
		m_window.draw(axis);

		sf::ConvexShape arrow;

		arrow.setPointCount(3);
		arrow.setFillColor(sf::Color::Black);

		//стрелка для оси y
		arrow.setPoint(0, sf::Vector2f(400.f, 0.f));
		arrow.setPoint(1, sf::Vector2f(392.f, 10.f));
		arrow.setPoint(2, sf::Vector2f(408.f, 10.f));
		m_window.draw(arrow);

		//стрелка для оси x
		arrow.setPoint(0, sf::Vector2f(800.f, 400.f));
		arrow.setPoint(1, sf::Vector2f(790.f, 392.f));
		arrow.setPoint(2, sf::Vector2f(790.f, 408.f));
		m_window.draw(arrow);
	}

	void plot_function(std::vector<float> y_array)
	{

		for (int i = -100; i < 100; ++i)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(sf::Vector2f(400 + (static_cast<float>(i) / 4) * 20, y_array[i + 100]), sf::Color::Blue),
				sf::Vertex(sf::Vector2f(400 + (static_cast<float>(i + 1 ) / 4) * 20, y_array[i + 101]), sf::Color::Blue)
			};
			m_window.draw(line, 2, sf::Lines);
		}
	}
};


class Get_mouse_coord
{
private:
	sf::RenderWindow& m_win;
	sf::Font& m_font;
	sf::Text m_x_text;
	sf::Text m_y_text;
	int m_t = 20;
public:
	Get_mouse_coord(sf::RenderWindow& win, sf::Font& font) : m_win(win), m_font(font)
	{
		x_change_coord_output(0);
		y_change_coord_output(0);
		render();
	}

	virtual void render()
	{
		m_x_text.setPosition(600, m_t);
		m_x_text.setFillColor(sf::Color::Black);
		m_x_text.setCharacterSize(13);
		m_x_text.setFont(m_font);
		m_win.draw(m_x_text);

		m_y_text.setPosition(700, m_t);
		m_y_text.setFillColor(sf::Color::Black);
		m_y_text.setCharacterSize(13);
		m_y_text.setFont(m_font);
		m_win.draw(m_y_text);
	
	}

	void auxiliary_axis(double x, double y)
	{
		// очь y
		if (x < 40) 
		{
			sf::Vertex y_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(40, 0), sf::Color::Red),
				sf::Vertex(sf::Vector2f(40, 780), sf::Color::Red)
			};
			m_win.draw(y_auxiliary_axis, 2, sf::Lines);
			x_change_coord_output(18);
		}
		else if (x > 780)
		{
			sf::Vertex y_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(780, 0), sf::Color::Red),
				sf::Vertex(sf::Vector2f(780, 780), sf::Color::Red)
			};
			m_win.draw(y_auxiliary_axis, 2, sf::Lines);
			x_change_coord_output(19);
		}
		else
		{
			sf::Vertex y_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(x, 0), sf::Color::Red),
				sf::Vertex(sf::Vector2f(x, 780), sf::Color::Red)
			};
			m_win.draw(y_auxiliary_axis, 2, sf::Lines);
			x_change_coord_output((x - 400) / 20);
		}

		//ось x
		if (y < 0)
		{
			sf::Vertex x_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(40, 1), sf::Color::Red),
				sf::Vertex(sf::Vector2f(780, 1), sf::Color::Red)
			};
			m_win.draw(x_auxiliary_axis, 2, sf::Lines);
			y_change_coord_output(20);
		}
		else if (y > 780)
		{
			sf::Vertex x_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(40, 780), sf::Color::Red),
				sf::Vertex(sf::Vector2f(780, 780), sf::Color::Red)
			};
			m_win.draw(x_auxiliary_axis, 2, sf::Lines);
			y_change_coord_output(-19);
		}
		else
		{
			sf::Vertex x_auxiliary_axis[] =
			{
				sf::Vertex(sf::Vector2f(40, y), sf::Color::Red),
				sf::Vertex(sf::Vector2f(780, y), sf::Color::Red)
			};
			m_win.draw(x_auxiliary_axis, 2, sf::Lines);
			y_change_coord_output((400 - y) / 20);
		}
		
	}

	void x_change_coord_output(double x)
	{
		m_x_text.setString("x: " + std::to_string(x));
	}
	void y_change_coord_output(double y)
	{
		m_y_text.setString("y: " + std::to_string(y));
	}
};


void new_main(std::string function)
{
	std::cout << function << std::endl;
	
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(800, 800), function, sf::Style::Close, settings);

	sf::Font font;
	font.loadFromFile("sfns-display-thin.ttf");

	Get_Coefficient Get_Coefficient(function);
	Get_Coefficient.func_split();
	Graphics Graphics(window, font, Get_Coefficient);
	Get_mouse_coord gmc(window, font);
	
	std::vector<float> y_array;
	y_array.resize(201);

	
	std::cout << "---------------------------------------------------------------------" << std::endl;
	for (int i = -100; i < 100; i++)
	{
		std::cout << "--------------\n " << i << std::endl;
		std::cout << 400 + (static_cast<float>(i) / 4) * 20 << ' ' << 400 - Get_Coefficient.get_y(static_cast<float>(i) / 4) * 20 << std::endl;
		y_array[i + 100] = 400 - Get_Coefficient.get_y(static_cast<float>(i) / 4) * 20;
	}
	
	
	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		Graphics.coordinate_axes();
		
		sf::Vector2i pos = sf::Mouse::getPosition(window);
		double x = pos.x;
		double y = pos.y;

		
		if ((x < 800 && x > 0) && (y < 800 && y > 0))
		{
			gmc.auxiliary_axis(x, y);
			gmc.render();
		}
		

		Graphics.plot_function(y_array);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.display();
	}
}

