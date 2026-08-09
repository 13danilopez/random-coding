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

struct ListWChar27 {
	int nelems = 0;
	ArrayWChar27 elem;
};

struct ListInt100 {
	int nelems = 0;
	ArrayInt100 elem;
};

const ArrayWChar27 ABECEDARIO = 
{
	L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I',
	L'J', L'K', L'L', L'M', L'N', L'Ñ', L'O', L'P', L'Q',
	L'R', L'S', L'T', L'U', L'V', L'W', L'X', L'Y', L'Z'
};

// =[FUNCIONES]==================================================================== //

void inicio(int& n_letras_palabra) 
{
	wcout << endl;
	wcout << L"¡Hola! Vamos a jugar a un juego de Ahorcado." << endl;
	wcout << L"Piensa en la palabra que quieres que intente adivinar." << endl;
	wcout << endl;
	wcout << L"Introduce el número de letras de tu palabra: ";
	wcin >> n_letras_palabra;
	while (n_letras_palabra <= 0) 
	{
		wcout << L"Número de letras no válido. Introduce el número de letras de tu palabra: ";
		wcin >> n_letras_palabra;
	}
	wcout << endl;
}

void inicializar_restantes(ListWChar27& letras_restantes)
{
	letras_restantes.elem = ABECEDARIO;
	letras_restantes.nelems = MAXLETRAS;
}

void inicializar_palabra(wstring& palabra, int n) 
{
	palabra = L"";
	for (int i = 0; i < n; ++i) { palabra.append(L"_"); }
}

void mostrar_palabra(const wstring& palabra, int n) 
{
	for (int i = 0; i < n; ++i) { wcout << palabra[i] << L" "; }
	wcout << endl;
	for (int i = 1; i <= n; ++i) { wcout << i << L" "; } // Mostrar índices debajo
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

inline bool es_respuesta_valida(wchar_t respuesta)
{
	return respuesta == L'y' || respuesta == L'Y' || respuesta == L'n' || respuesta == L'N';
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
	
	if (lista_comas.nelems == 0)
	{ 	// Si solo es una posición...
		lista_posiciones.elem[0] = stoi(str_posiciones);
		lista_posiciones.nelems++;
	}
	else
	{	// Si son varias posiciones...
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
			aux_wstring = str_posiciones.substr(pos_ini, pos_fin - pos_ini + 1);	// Extraer subcadena
			int posicion = stoi(aux_wstring);					// Conversión a int con stoi()
			lista_posiciones.elem[lista_posiciones.nelems] = posicion;		// Añadir a lista de posiciones
			lista_posiciones.nelems++;
		}
	}
	return lista_posiciones;
}

void actualizar_palabra(wstring& palabra, const ListInt100& lista_posiciones, wchar_t letra_adivinada)
{
	for (int i = 0; i < lista_posiciones.nelems; ++i)
	{
		int posicion = lista_posiciones.elem[i] - 1;
		if (palabra[posicion] != L'_')
		{
			wcout << L"Error. Ya hay adivinada la letra en la posición " << posicion << L"." << endl;
			exit(-1);
		}
		if (posicion < 0 || posicion > palabra.length()-1)
		{
			wcout << L"Error. La posición " << posicion << L" está fuera del rango de la palabra." << endl;
			exit(-1);
		}
		palabra[posicion] = letra_adivinada;
	}
}

int get_pos_wchar_t_list(const ListWChar27& lista_wchar_t, wchar_t letra)
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
	  	int pos = get_pos_wchar_t_list(letras_restantes, letra_adivinada);
		letras_restantes.elem[pos] = letras_restantes.elem[letras_restantes.nelems - 1];
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

void linea()
{
	for (int i = 0; i < LONGLINEA; ++i) { wcout << L"="; }
	wcout << endl << endl;
}

void ahorcado(wstring& palabra, int n_letras_palabra, int& cnt_intentos, bool& adivinada) 
{
	bool fin = false;
	ListWChar27 letras_restantes;	// Guardar las letras que quedan por adivinar (evitar repeticiones)
	ListWChar27 letras_adivinadas;	// Guardar las letras ya adivinadas
	
	cnt_intentos = 0;		// Inicializar parámetros pasados por referencia
	adivinada = false;

	inicializar_restantes(letras_restantes);	// Inicializar las restantes al abecedario inicial definido
	inicializar_palabra(palabra, n_letras_palabra);	// Inicializar la palabra según el número de letras indicado

	while (!adivinada && !fin)
	{
		linea();

		mostrar_palabra(palabra, n_letras_palabra);					// [1] Mostrar estado del juego
		wcout << endl;
		mostrar_restantes(letras_restantes);
		mostrar_adivinadas(letras_adivinadas);
		wcout << endl;

		wchar_t letra_adivinada = adivinar_letra(letras_restantes); 			// [2] Adivinar una letra de las restantes y preguntar
		wchar_t respuesta;
		do {										// (*) Bucle en caso de respuesta no válida (distinta de y/N)
			wcout << L"¿Contiene tu palabra la letra [" << letra_adivinada << L"]? (y/N): ";
			wcin >> respuesta;
			std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 	// (*) Limpiar input buffer (en caso de poner varias letras)
			if (respuesta == L'y' || respuesta == L'Y')				// [3] Si la respuesta es SÍ => preguntar posiciones
			{
				wcout << L"Indica en qué posiciones hay una [" << letra_adivinada << L"] (e.g. 1,4,7): ";
				wstring str_posiciones;
				wcin >> str_posiciones;
				ListInt100 lista_posiciones = parse_posiciones(str_posiciones); // [4] Parseo de posiciones del wstring dado
				actualizar_palabra(palabra, lista_posiciones, letra_adivinada);	// [5] Actualizar la palabra de acuerdo con las posiciones
			}
		} while (!es_respuesta_valida(respuesta));
		
		actualizar_restantes(letras_restantes, letra_adivinada);			// [6] Actualizar lista de letras restantes (eliminar)
		actualizar_adivinadas(letras_adivinadas, letra_adivinada);			// [7] Actualizar lista de letras adivinadas (añadir)

		cnt_intentos++;									// [8] Incrementar número de intentos usados
		if (palabra_completa(palabra)) adivinada = true;				// [9] Checkear si la palabra ya ha sido adivinada
		if (letras_restantes.nelems == 0) fin = true;					// [10] Checkear en caso de quedarse sin letras (por si no acierta)
		
		wcout << endl;
	}
	
	linea();
}

void fin(const wstring& palabra, int cnt_intentos, bool adivinada)
{
	if (adivinada)
	{	// Si ha sido adivinada...
		wcout << L"¡La adiviné! Tu palabra es: " << palabra << endl;
		wcout << L"Número de intentos: " << cnt_intentos << endl;
	}
	else
	{	// Si no ha sido adivinada...
		wcout << L"¡Imposible! ¡Tienes que haberte equivocado!" << endl;
	}
}

// =[MAIN]========================================================================= //

int main() 
{
	std::locale::global(std::locale(""));	// Configurar soporte de caracteres especiales en la consola
	std::wcout.imbue(std::locale(""));
	std::wcin.imbue(std::locale(""));

	wstring palabra;
	int n_letras_palabra;
	int cnt_intentos;
	bool adivinada;

	inicio(n_letras_palabra);
	ahorcado(palabra, n_letras_palabra, cnt_intentos, adivinada);
	fin(palabra, cnt_intentos, adivinada);

	return 0;
}
