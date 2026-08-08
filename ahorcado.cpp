#include <iostream>
#include <array>
#include <bits/stdc++.h>

using namespace std;

// ================================================================================ //

const int MAXLETRAS = 27;
const int MAXNUMS = 100;
const int LONGLINEA = 75;

typedef array<char, MAXLETRAS>ArrayChar27;
typedef array<int, MAXNUMS>ArrayInt100;

struct ListChar27 {
	int nelems = 0;
	ArrayChar27 elem;
};

struct ListInt100 {
	int nelems = 0;
	ArrayInt100 elem;
};

const ArrayChar27 ABECEDARIO = 
{
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
	'J', 'K', 'L', 'M', 'N', '?', 'O', 'P', 'Q',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

// ================================================================================ //

void inicio(int& n_letras_palabra) 
{
	cout << endl;
	cout << "¡Hola! Vamos a jugar a un juego de Ahorcado." << endl;
	cout << "Piensa en la palabra que quieres que intente adivinar." << endl;
	cout << endl;
	cout << "Introduce el número de letras de tu palabra: ";
	cin >> n_letras_palabra;
	while (n_letras_palabra <= 0) 
	{
		cout << "Número de letras no válido. Introduce el número de letras de tu palabra: ";
		cin >> n_letras_palabra;
	}
	cout << endl;
}

void inicializar_restantes(ListChar27& letras_restantes)
{
	letras_restantes.elem = ABECEDARIO;
	letras_restantes.nelems = MAXLETRAS;
}

void inicializar_palabra(string& palabra, int n) 
{
	palabra = "";
	for (int i = 0; i < n; ++i) { palabra.append("_"); }
}

void mostrar_palabra(const string& palabra, int n) 
{
	for (int i = 0; i < n; ++i) { cout << palabra[i] << " "; }
	cout << endl;
	for (int i = 1; i <= n; ++i) { cout << i << " "; } // Mostrar índices debajo
	cout << endl;
}

void mostrar_restantes(const ListChar27& letras_restantes)
{
	cout << "Letras restantes: ";
	for (int i = 0; i < letras_restantes.nelems; ++i) 
	{
		cout << letras_restantes.elem[i] << " ";
	}
	cout << endl;
}

void mostrar_adivinadas(const ListChar27& letras_adivinadas)
{
	cout << "Letras adivinadas: ";
	for (int i = 0; i < letras_adivinadas.nelems; ++i)
	{
		cout << letras_adivinadas.elem[i] << " ";
	}
	cout << endl;
}

char adivinar_letra(const ListChar27& letras_restantes) 
{
	srand(time(0));						// Seed random basada en el tiempo actual
	int rand_int = rand() % letras_restantes.nelems; 	// Entero random entre 0..length-1

	if (rand_int < 0 || rand_int > (letras_restantes.nelems - 1))	// Confirmar validez del rand_int
	{
		cout << "Error. Letra seleccionada al azar fuera de rango." << endl;
		cout << "Letras restantes: ";
		for (int i = 0; i < letras_restantes.nelems; ++i)
		{
			cout << letras_restantes.elem[i] << " ";
		}
		cout << endl;
		cout << "Número letras restantes: " << letras_restantes.nelems << endl;
		cout << "Número de letra adivinada: " << rand_int << endl;
		exit(-1);
	}

	return letras_restantes.elem[rand_int];
}

inline bool es_respuesta_valida(char respuesta)
{
	return respuesta == 'y' || respuesta == 'Y' || respuesta == 'n' || respuesta == 'N';
}

ListInt100 parse_comas(string string_posiciones)
{
	ListInt100 lista_comas;
	for (int i = 0; i < string_posiciones.length(); ++i)
	{
		if (string_posiciones[i] == ',')
		{
			lista_comas.elem[lista_comas.nelems] = i;
			lista_comas.nelems++;
		}
	}
	return lista_comas;
}

ListInt100 parse_posiciones(string str_posiciones)
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
			string aux_string;
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
			aux_string = str_posiciones.substr(pos_ini, pos_fin - pos_ini + 1);	// Extraer subcadena
			int posicion = stoi(aux_string);					// Conversión a int con stoi()
			lista_posiciones.elem[lista_posiciones.nelems] = posicion;		// Añadir a lista de posiciones
			lista_posiciones.nelems++;
		}
	}
	return lista_posiciones;
}

void actualizar_palabra(string& palabra, const ListInt100& lista_posiciones, char letra_adivinada)
{
	for (int i = 0; i < lista_posiciones.nelems; ++i)
	{
		int posicion = lista_posiciones.elem[i] - 1;
		if (palabra[posicion] != '_')
		{
			cout << "Error. Ya hay adivinada la letra en la posición " << posicion << "." << endl;
			exit(-1);
		}
		if (posicion < 0 || posicion > palabra.length()-1)
		{
			cout << "Error. La posición " << posicion << " está fuera del rango de la palabra." << endl;
			exit(-1);
		}
		palabra[posicion] = letra_adivinada;
	}
}

int get_pos_char_list(const ListChar27& lista_char, char letra)
{
	bool encontrada = false;
	int i = 0;
	while (!encontrada && i < lista_char.nelems)
	{
		if (lista_char.elem[i] == letra) encontrada = true;
		else ++i;
	}

	if (!encontrada)
	{
		cout << "Error. La letra " << letra << " no pertenece a la lista." << endl;
		exit(-1);
	}

	return i;
}

void actualizar_restantes(ListChar27& letras_restantes, char letra_adivinada)
{
	if (letras_restantes.nelems <= 1)
	{
		letras_restantes.elem = {};
		letras_restantes.nelems = 0;
	}
	else
	{ 	// Sustituir letra a eleminar por la última, y decrementar tamaño
	  	int pos = get_pos_char_list(letras_restantes, letra_adivinada);
		letras_restantes.elem[pos] = letras_restantes.elem[letras_restantes.nelems - 1];
		letras_restantes.nelems--;
	}
}

void actualizar_adivinadas(ListChar27& letras_adivinadas, char letra_adivinada)
{
	if (letras_adivinadas.nelems > MAXLETRAS)
	{
		cout << "Error. Lista de letras adivinadas supera tamaño máximo." << endl;
		cout << "Letras adivinadas: ";
		for (int i = 0; i < letras_adivinadas.nelems; ++i)
		{
			cout << letras_adivinadas.elem[i] << " ";
		}
		cout << endl;
		cout << "Número letras adivinadas: " << letras_adivinadas.nelems << endl;
		exit(-1);
	}
	else
	{	// Añadir letra al final de la lista
		letras_adivinadas.elem[letras_adivinadas.nelems] = letra_adivinada;
		letras_adivinadas.nelems++;
	}

}

bool palabra_completa(const string& palabra)
{
	bool ok = true;
	int i = 0;
	while (ok && i < palabra.length())
	{
		if (palabra[i] == '_') ok = false;
		++i;
	}
	return ok;
}

void linea()
{
	for (int i = 0; i < LONGLINEA; ++i) { cout << "="; }
	cout << endl << endl;
}

void ahorcado(string& palabra, int n_letras_palabra, int& cnt_intentos, bool& adivinada) 
{
	bool fin = false;
	ListChar27 letras_restantes;	// Guardar las letras que quedan por adivinar (evitar repeticiones)
	ListChar27 letras_adivinadas;	// Guardar las letras ya adivinadas
	
	cnt_intentos = 0;		// Inicializar parámetros pasados por referencia
	adivinada = false;

	inicializar_restantes(letras_restantes);	// Inicializar las restantes al abecedario inicial definido
	inicializar_palabra(palabra, n_letras_palabra);	// Inicializar la palabra según el número de letras indicado

	while (!adivinada && !fin)
	{
		linea();

		mostrar_palabra(palabra, n_letras_palabra);					// [1] Mostrar estado del juego
		cout << endl;
		mostrar_restantes(letras_restantes);
		mostrar_adivinadas(letras_adivinadas);
		cout << endl;

		char letra_adivinada = adivinar_letra(letras_restantes); 			// [2] Adivinar una letra de las restantes y preguntar
		char respuesta;
		do {										// (*) Bucle en caso de respuesta no válida (distinta de y/N)
			cout << "¿Contiene tu palabra la letra [" << letra_adivinada << "]? (y/N): ";
			cin >> respuesta;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 	// (*) Limpiar input buffer (en caso de poner varias letras)
			if (respuesta == 'y' || respuesta == 'Y')				// [3] Si la respuesta es SÍ => preguntar posiciones
			{
				cout << "Indica en qué posiciones hay una [" << letra_adivinada << "] (e.g. 1,4,7): ";
				string str_posiciones;
				cin >> str_posiciones;
				ListInt100 lista_posiciones = parse_posiciones(str_posiciones); // [4] Parseo de posiciones del string dado
				actualizar_palabra(palabra, lista_posiciones, letra_adivinada);	// [5] Actualizar la palabra de acuerdo con las posiciones
			}
		} while (!es_respuesta_valida(respuesta));
		
		actualizar_restantes(letras_restantes, letra_adivinada);			// [6] Actualizar lista de letras restantes (eliminar)
		actualizar_adivinadas(letras_adivinadas, letra_adivinada);			// [7] Actualizar lista de letras adivinadas (añadir)

		cnt_intentos++;									// [8] Incrementar número de intentos usados
		if (palabra_completa(palabra)) adivinada = true;				// [9] Checkear si la palabra ya ha sido adivinada
		if (letras_restantes.nelems == 0) fin = true;					// [10] Checkear en caso de quedarse sin letras (por si no acierta)
		
		cout << endl;
	}
	
	linea();
}

void fin(const string& palabra, int cnt_intentos, bool adivinada)
{
	if (adivinada)
	{	// Si ha sido adivinada...
		cout << "¡La adiviné! Tu palabra es: " << palabra << endl;
		cout << "Número de intentos: " << cnt_intentos << endl;
	}
	else
	{	// Si no ha sido adivinada...
		cout << "¡Imposible! ¡Tienes que haberte equivocado!" << endl;
	}
}

// ================================================================================ //

int main() 
{
	string palabra;
	int n_letras_palabra;
	int cnt_intentos;
	bool adivinada;

	inicio(n_letras_palabra);
	ahorcado(palabra, n_letras_palabra, cnt_intentos, adivinada);
	fin(palabra, cnt_intentos, adivinada);

	return 0;
}
