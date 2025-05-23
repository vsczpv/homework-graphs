
#ifndef CODE_VIN_HPP_
#define CODE_VIN_HPP_

#include <string>
#include <tuple>
#include <time.h>
#include <cstdlib>

/* Desdefinir macros do Visual Studio */
#undef max
#undef min

/* Utilitários */
namespace vin
{
	// Ask the user a value
	// Currently instanciated: int, double, std::string, char
	template <typename T> extern T ask(std::string prompt);

	/* Escape de posicionar cursor */
	std::string vt_pos (int x, int y);

	/*
	 * Ncurses não emite o carriage-return quando usado com std::cout,
	 * causando um "efeito escadinha" durante uma nova linha.
	 * É nescessário reimplementar o std::endl para emitir '\r';
	 */
	template <class CharT, class Traits>
	std::basic_ostream <CharT, Traits>& endl (std::basic_ostream<CharT,Traits>& os)
	{
		os.put   (os.widen ('\r'));
		os.put   (os.widen ('\n'));
		os.flush ();
		return os;
	}

	constexpr bool NO_CURSES = false;

	/* Funções de compatibilidade */
	void compat_prologue(bool setup_io);
	void compat_epilogue(void);

	/* Wrapper envolta do ncurses/conio.h */
	namespace fakecurses
	{
		int  getc   (void);
		void refr (void);
	}

	template <typename T, typename U> class zip
	{
	public:
		class iterator
		{
		private:
			typename T::iterator x;
			typename U::iterator y;
		public:
			iterator (typename T::iterator x, typename U::iterator y) : x (x), y (y) {}
			bool operator!=(const iterator& other) { return x != other.x; }
			iterator& operator++()                 { x++, y++; return *this; }
			iterator  operator++(int)              { auto res = *this; ++(*this); return res; }
			iterator& operator--()                 { x--, y--; return *this; }
			auto operator*()                       { return std::tuple (*x, *y); }
		};

		T& x;
		U& y;

		zip (T& x, U& y) : x (x), y (y)
		{
			if (x.size() != y.size()) throw std::logic_error ("both operands must have same length");
		}

		iterator begin (void) { return iterator (x.begin(), y.begin()); }
		iterator end   (void) { return iterator (x.end(),   y.end()  ); }
	};

}

/* Escape para limpar tela */
#define VT_CLEAR     "\033[H\033[2J"

/* Escapes de cor */
#define VT_END       "\033[0m"
#define VT_BOLD      "\033[1m"
#define VT_BLINK     "\033[5m"
#define VT_RED       "\033[31m"
#define VT_GREEN     "\033[32m"
#define VT_YELLOW    "\033[33m"
#define VT_BLUE      "\033[94m"
#define VT_WHITE     "\033[37m"
#define VT_BROWN     "\033[38;2;63;18;0m"
#define VT_ORANGE_BG "\033[48;2;126;36;0m"
#define VT_GRAY      "\033[38;2;120;120;120m"
#define VT_GRAY_BG   "\033[48;2;120;120;120m"

/* Escape de posicionar cursor */
std::string vt_pos (int x, int y);

/* Escapes de visibilidade do cursor */
#define VT_HIDE_CUR "\033[?25l"
#define VT_SHOW_CUR "\033[?25h"


#include <iostream>
#include <limits>

template <typename T>
T vin::ask(std::string prompt)
{

	T val;

	std::cout << prompt;

	while ( !(std::cin >> val) )
	{
		std::cout << prompt;
		std::cin.clear();
#pragma push_macro("max")
#undef max
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#pragma pop_macro("max")
	}
	
	return val;
}

template int         vin::ask<int>         (std::string);
template std::size_t vin::ask<std::size_t> (std::string);
template double      vin::ask<double>      (std::string);
template std::string vin::ask<std::string> (std::string);
template char        vin::ask<char>        (std::string);
template long int    vin::ask<long int>    (std::string);

/* Windows usa CMD.exe + conio.h, enquanto *NIX usa ncurses */
#if defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64

#ifndef WIN32
#define WIN32
#endif

#include <windows.h>
#include <conio.h>

#undef min
#undef max

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#else

#include <curses.h>

#endif

/*
 * Função de compatibilidade
 * Ativa a formatação VT do CMD.exe (Windows), e o ncurses em modo filter (*NIX)
 */
void vin::compat_prologue (bool setup_io = false)
{

	srand(time(nullptr));

#ifdef WIN32

	setlocale(LC_ALL, "pt_BR.UTF-8");

	SetConsoleOutputCP( 65001 );

	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}

#else

	if (setup_io)
	{
		filter  ();
		initscr ();
		noecho  ();
		refresh ();
	}

#endif

	return;

}

/*
 * Função de compatibilidade
 * Desativa o ncurses (*NIX)
 */
void vin::compat_epilogue (void)
{

#ifdef WIN32

#else

	endwin ();

#endif

	return;
}

int vin::fakecurses::getc (void)
{
	return getch ();
}

void vin::fakecurses::refr (void)
{

#ifdef WIN32

#else
	refresh ();
#endif

}

std::string vin::vt_pos(int x, int y)
{
	x += 1, y += 1;
	return "\033[" + std::to_string(y) + ";" + std::to_string(x) + "H";
}

#endif // CODE_VIN_HPP_

