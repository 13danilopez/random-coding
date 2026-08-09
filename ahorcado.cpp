#include <iostream>
#include <array>
#include <bits/stdc++.h>

using namespace std;

// =[TIPOS]======================================================================== //

const int MAXLETRAS = 27;
const int MAXNUMS = 100;
const int LONGLINEA = 75;

typedef array<wchar_t, MAXLETRAS>ArrayWChar27;
typedef array<int, MAXNUMS>ArrayInt100;

struct ListWChar27 
{
	int nelems = 0;
	ArrayWChar27 elem;
};

struct ListInt100 
{
	int nelems = 0;
	ArrayInt100 elem;
};

const ArrayWChar27 ABECEDARIO = 
{
	L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I',
	L'J', L'K', L'L', L'M', L'N', L'Ñ', L'O', L'P', L'Q',
	L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y', L'Z'
};

struct GameState
{
	wstring palabra;
	int n_intentos;
	bool adivinada;
	ListWChar27 let_res;
	ListWChar27 let_adv;
};

// =[FUNCIONES]==================================================================== //

void inicializar_gamestate(GameState& gs)
{
	gs.palabra		= L"";
	gs.n_intentos		= 0;
	gs.adivinada		= false;
	gs.let_res.elem		= ABECEDARIO;
	gs.let_res.nelems	= MAXLETRAS;
	gs.let_adv.elem		= {};
	gs.let_adv.nelems	= 0;
}

void linea()
{
	wcout << endl;
	for (int i = 0; i < LONGLINEA; ++i) { wcout << L"="; }
	wcout << endl << endl;
}

void inicializar_palabra(GameState& gs, int n_letras) 
{
	for (int i = 0; i < n_letras; ++i) { gs.palabra.append(L"_"); }
}

void inicio(GameState& gs) 
{
	linea();
	wcout << L"¡Hola! Vamos a jugar a un juego de Ahorcado." << endl;
	wcout << L"Piensa en la palabra que quieres que intente adivinar." << endl;
	wcout << endl;

	int n_letras;
	do {
		wcout << L"Introduce el número de letras de tu palabra (1..27): ";
		wcin >> n_letras;
	} while (n_letras < 1 || n_letras > MAXLETRAS);

	inicializar_palabra(gs, n_letras);
}

void mostrar_palabra(const wstring& palabra) 
{
	for (int i = 0; i < palabra.length(); ++i) { wcout << palabra[i] << L" "; }
	wcout << endl;
	for (int idx = 1; idx <= palabra.length(); ++idx) { wcout << idx << L" "; } // Mostrar índices debajo
	wcout << endl;
}

void mostrar_restantes(const ListWChar27& letras_restantes)
{
	wcout << L"Letras restantes: ";
	for (int i = 0; i < letras_restantes.nelems; ++i) 
	{
		wcout << letras_restantes.elem[i] << L" ";
	}
	wcout << endl;
}

void mostrar_adivinadas(const ListWChar27& letras_adivinadas)
{
	wcout << L"Letras adivinadas: ";
	for (int i = 0; i < letras_adivinadas.nelems; ++i)
	{
		wcout << letras_adivinadas.elem[i] << L" ";
	}
	wcout << endl;
}

void mostrar_gamestate(GameState& gs)
{
	linea();
	mostrar_palabra(gs.palabra);
	wcout << endl;
	mostrar_restantes(gs.let_res);
	mostrar_adivinadas(gs.let_adv);
	wcout << endl;
}

wchar_t adivinar_letra(const ListWChar27& letras_restantes) 
{
	srand(time(0));						// Seed random basada en el tiempo actual
	int rand_int = rand() % letras_restantes.nelems; 	// Entero random entre 0..length-1

	if (rand_int < 0 || rand_int > (letras_restantes.nelems - 1))	// Confirmar validez del rand_int
	{
		wcout << L"Error. Letra seleccionada al azar fuera de rango." << endl;
		wcout << L"Letras restantes: ";
		for (int i = 0; i < letras_restantes.nelems; ++i)
		{
			wcout << letras_restantes.elem[i] << L" ";
		}
		wcout << endl;
		wcout << L"Número letras restantes: " << letras_restantes.nelems << endl;
		wcout << L"Número de letra adivinada: " << rand_int << endl;
		exit(-1);
	}

	return letras_restantes.elem[rand_int];
}

ListInt100 parse_comas(wstring wstring_posiciones)
{
	ListInt100 lista_comas;
	for (int i = 0; i < wstring_posiciones.length(); ++i)
	{
		if (wstring_posiciones[i] == L',')
		{
			lista_comas.elem[lista_comas.nelems] = i;
			lista_comas.nelems++;
		}
	}
	return lista_comas;
}

ListInt100 parse_posiciones(wstring str_posiciones)
{
	ListInt100 lista_posiciones;
	ListInt100 lista_comas;
	lista_comas = parse_comas(str_posiciones);	// Las posiciones de las comas y el número de comas son usadas para sacar los números
	
	int n_posiciones = lista_comas.nelems + 1;	// Número de posiciones dado por num_comas+1
	for (int i = 0; i < n_posiciones; ++i)	
	{
		wstring aux_wstring;
		int pos_ini = -1;	// Sacamos la substr sabiendo la pos_ini y la pos_fin
		int pos_fin = -1;	// Usaremos las posiciones de las comas que preceden y suceden para esto
		if (i == 0)
		{	// Si es el primer número... [CASO BASE]
			pos_ini = 0;
			pos_fin = lista_comas.elem[0] - 1;
		}
		else if (i == lista_comas.nelems)
		{	// Si es el último número... [CASO BASE]
			pos_ini = lista_comas.elem[lista_comas.nelems-1] + 1;
			pos_fin = str_posiciones.length() - 1;
		}
		else
		{	// Si está en medio... [CASO GENERAL]
			pos_ini = lista_comas.elem[i-1] + 1;
			pos_fin = lista_comas.elem[i] - 1;
		}

		try {
			aux_wstring = str_posiciones.substr(pos_ini, pos_fin - pos_ini + 1);	// Extraer subcadena
			int posicion = stoi(aux_wstring);					// Conversión a int con stoi()
			lista_posiciones.elem[lista_posiciones.nelems] = posicion;		// Añadir a lista de posiciones
			lista_posiciones.nelems++;
		} catch (exception e) {
			lista_posiciones.elem = {};
			lista_posiciones.nelems = 0;
			return lista_posiciones;
		}
	}
	return lista_posiciones;
}

void actualizar_palabra(wstring& palabra, const wstring& preview_palabra)
{
	palabra = preview_palabra;
}

int get_pos_wchar_list(const ListWChar27& lista_wchar_t, wchar_t letra)
{
	bool encontrada = false;
	int i = 0;
	while (!encontrada && i < lista_wchar_t.nelems)
	{
		if (lista_wchar_t.elem[i] == letra) encontrada = true;
		else ++i;
	}

	if (!encontrada)
	{
		wcout << L"Error. La letra " << letra << L" no pertenece a la lista." << endl;
		exit(-1);
	}

	return i;
}

void actualizar_restantes(ListWChar27& letras_restantes, wchar_t letra_adivinada)
{
	if (letras_restantes.nelems <= 1)
	{
		letras_restantes.elem = {};
		letras_restantes.nelems = 0;
	}
	else
	{ 	// Sustituir letra a eleminar por la última, y decrementar tamaño
	  	int pos = get_pos_wchar_list(letras_restantes, letra_adivinada);
		for (int i = pos+1; i < letras_restantes.nelems; ++i)
		{
			wchar_t aux = letras_restantes.elem[i];
			letras_restantes.elem[i] = letras_restantes.elem[i-1];
			letras_restantes.elem[i-1] = aux;
		}
		letras_restantes.nelems--;
	}
}

void actualizar_adivinadas(ListWChar27& letras_adivinadas, wchar_t letra_adivinada)
{
	if (letras_adivinadas.nelems > MAXLETRAS)
	{
		wcout << L"Error. Lista de letras adivinadas supera tamaño máximo." << endl;
		wcout << L"Letras adivinadas: ";
		for (int i = 0; i < letras_adivinadas.nelems; ++i)
		{
			wcout << letras_adivinadas.elem[i] << L" ";
		}
		wcout << endl;
		wcout << L"Número letras adivinadas: " << letras_adivinadas.nelems << endl;
		exit(-1);
	}
	else
	{	// Añadir letra al final de la lista
		letras_adivinadas.elem[letras_adivinadas.nelems] = letra_adivinada;
		letras_adivinadas.nelems++;
	}

}

bool palabra_completa(const wstring& palabra)
{
	bool ok = true;
	int i = 0;
	while (ok && i < palabra.length())
	{
		if (palabra[i] == L'_') ok = false;
		++i;
	}
	return ok;	
}

bool get_contiene_letra(wchar_t letra_adivinada)
{
	bool contiene = false;
	bool respuesta_valida = false;
	wchar_t respuesta;
	do {
		wcout << L"¿Contiene tu palabra la letra [" << letra_adivinada << L"]? (y/N): ";
		wcin >> respuesta;
		std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 	// (*) Limpiar input buffer (en caso de poner varias letras)
		
		if 	(respuesta == L'y' || respuesta == L'Y') 
		{
			contiene = true;  respuesta_valida = true;
		}
		else if (respuesta == L'n' || respuesta == L'N') 
		{
			contiene = false; respuesta_valida = true;
		}
	} while (!respuesta_valida);
	return contiene;
}

bool validar_posiciones(const ListInt100& lista_posiciones, const wstring& palabra)
{
	if (lista_posiciones.nelems == 0) return false;

	int min = 1;
	int max = palabra.length();
	
	bool validas = true;
	int i = 0;
	while (validas && i < lista_posiciones.nelems)
	{
		int pos = lista_posiciones.elem[i];
		if (pos < min || pos > max || palabra[pos-1] != L'_') validas = false;
		++i;
	}

	return validas;
}

ListInt100 get_posiciones_letra(const wstring& palabra, wchar_t letra_adivinada)
{
	ListInt100 lista_posiciones;
	bool posiciones_validas = false;
	do {
		wcout << L"Indica en qué posiciones hay una [" << letra_adivinada << L"] (e.g. 1,4,7): ";
		wstring str_posiciones;
		wcin >> str_posiciones;
		lista_posiciones = parse_posiciones(str_posiciones);	// Si hay algún error en el parsing --> devuelve lista empty
		posiciones_validas = validar_posiciones(lista_posiciones, palabra);
	} while (!posiciones_validas);
	return lista_posiciones;
}

wstring get_preview_palabra(const wstring& palabra, const ListInt100& lista_posiciones, wchar_t letra_adivinada)
{
	wstring preview_palabra = palabra;
	for (int i = 0; i < lista_posiciones.nelems; ++i)
	{
		int posicion = lista_posiciones.elem[i] - 1;
		preview_palabra[posicion] = letra_adivinada;
	}
	return preview_palabra;
}


bool get_confirmacion_preview(const wstring& preview_palabra)
{
	bool ok_preview = false;
	bool respuesta_valida = false;
	wchar_t respuesta;
	do {
		wcout << L"¿Confirmar [ "; 
		for (int i = 0; i < preview_palabra.length(); ++i) { wcout << preview_palabra[i] << L" "; }
		wcout << L"]? (y/N): ";
		wcin >> respuesta;
		std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 	// (*) Limpiar input buffer (en caso de poner varias letras)
		
		if 	(respuesta == L'y' || respuesta == L'Y') 
		{
			ok_preview = true;  respuesta_valida = true;
		}
		else if (respuesta == L'n' || respuesta == L'N') 
		{
			ok_preview = false; respuesta_valida = true;
		}
	} while (!respuesta_valida);
	return ok_preview;
}



void ahorcado(GameState& gs) 
{
	// VARIABLES & ESTRUCTURAS
	bool fin = false;
		
	// GAME LOOP
	while (!gs.adivinada && !fin)
	{
		mostrar_gamestate(gs);						// [1] Mostrar estado del juego
		
		wchar_t letra_adivinada = adivinar_letra(gs.let_res); 		// [2] Adivinar una letra de las restantes
		bool contiene = get_contiene_letra(letra_adivinada);		// [3] Preguntar si la palabra contiene la letra
		if (contiene)							// [4] Si la respuesta es SÍ...
		{
			ListInt100 lista_posiciones;
			wstring preview_palabra;
			bool ok_preview;
			do {
				lista_posiciones = get_posiciones_letra(gs.palabra, letra_adivinada);			// [4.1] Preguntar posiciones
				preview_palabra = get_preview_palabra(gs.palabra, lista_posiciones, letra_adivinada);	// [4.2] Crear una preview
				ok_preview = get_confirmacion_preview(preview_palabra);					// [4.3] Preguntar confirmación
			} while (!ok_preview);										// [*] Bucle para confirmación

			actualizar_palabra(gs.palabra, preview_palabra);			// [4.4] Una vez confirmado, actualizar la palabra real
		}
				
		actualizar_restantes(gs.let_res, letra_adivinada);		// [5] Actualizar lista de letras restantes (eliminar)
		actualizar_adivinadas(gs.let_adv, letra_adivinada);		// [6] Actualizar lista de letras adivinadas (añadir)

		gs.n_intentos++;						// [7] Incrementar número de intentos usados
		if (palabra_completa(gs.palabra)) gs.adivinada = true;		// [8] Checkear si la palabra ya ha sido adivinada
		if (gs.let_res.nelems == 0) fin = true;				// [9] Checkear en caso de quedarse sin letras (por si no acierta)
	}
}

void fin(const GameState& gs)
{
	linea();
	if (gs.adivinada)
	{	// Si ha sido adivinada...
		wcout << L"¡La adiviné! Tu palabra es: " << gs.palabra << endl;
		wcout << L"Número de intentos: " << gs.n_intentos << endl;
	}
	else
	{	// Si no ha sido adivinada...
		wcout << L"¡Imposible! ¡Tienes que haberte equivocado!" << endl;
	}
	linea();
}

// =[MAIN]========================================================================= //

int main() 
{
	std::locale::global(std::locale(""));	// Configurar soporte de caracteres especiales en la consola
	std::wcout.imbue(std::locale(""));
	std::wcin.imbue(std::locale(""));
	
	GameState gs;
	inicializar_gamestate(gs);

	inicio(gs);
	ahorcado(gs);
	fin(gs);

	return 0;
}
