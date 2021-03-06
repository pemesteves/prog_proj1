// program1.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstddef>
#include <algorithm>

using namespace std;

//Declaração das funções
void extract_words(vector <string> &vec, ifstream &f);

void save_words(const vector<string> &vec, ofstream &file);

bool validLine(string line);
//Fim da declaração das funções

int main()
{
	cout << "EXTRACTION OF WORD LIST FROM DICTIONARY" << endl;
	cout << "=======================================" << endl;

	//Abertura do ficheiro que contém o dicionário
	cout << "Dictionary file ? ";
	string dictionaryFileName;
	cin >> dictionaryFileName;
	ifstream dictionary(dictionaryFileName);

	if (dictionary.fail())
	{
		cerr << "Input file opening failed.\n";
		exit(1); //por vezes é preferível parar o programa com um exit code != 0
	}

	//Nome do ficheiro que será criado para guardar as palavras
	cout << "Word list file ? ";
	string wordsFileName;
	cin >> wordsFileName;

	cout << "Extracting simple words from file " << dictionaryFileName << "," << endl;
	cout << "beginning with letter ..." << endl << endl;


	//Extração das Palavras
	char firstLetter = 'A';
	cout << firstLetter << endl;
	vector <string> words;
	extract_words(words, dictionary);
	dictionary.close();


	cout << endl << "Number of simple words = " << words.size() << endl << endl;

	//Ordenação do vetor onde se encontram as palavras
	cout << "Sorting words ..." << endl << endl;
	sort(words.begin(), words.end());


	//Remoção das palavras duplicadas
	cout << "Removing duplicate words ..." << endl << endl;
	words.erase(unique(words.begin(), words.end()), words.end());

	cout << "Number of non-duplicate simple words = " << words.size() << endl << endl;
	
	//Abertura do ficheiro onde as palavras do vetor serão guardadas
	ofstream wordsFile(wordsFileName);

	cout << "Saving words into file " << wordsFileName << " ..." << endl;
	save_words(words, wordsFile);

	wordsFile.close();

	cout << endl << "End of processing." << endl;
	
	//Fim do programa

	system("pause"); //Para deixar o utilizador fechar a janela e conseguir ver o fim do programa

	return 0;
}

//Função que extrai palavras do ficheiro selecionado pelo utilizador caso este seja válido
void extract_words(vector <string> &vec, ifstream &f)
{
	int numberWords = 0; //Contador de Palavras			
	int letter = 65; //ASCII code of A
	string LineOfFile; //Linha a ser extraída do ficheiro

	string substrOfLine; //Substring da linha que será analisada
	while (!f.eof())
	{
		getline(f, LineOfFile);
		
		if (validLine(LineOfFile)) //Se a linha é válida; caso não seja, não faz nada
		{
			if (LineOfFile.at(0) == (char)letter + 1) //Caso a 1ª letra da linha seja a letra seguinte há que está a ser considerada, passa-se para a letra seguinte
			{
				letter++;
				cout << endl << (char)letter << endl;
				numberWords = 0;
			}

			while (true)
			{
				if (LineOfFile.find(';') == string::npos) //Caso já não haja ';' na linha
				{
					if (LineOfFile.find(' ') == string::npos && LineOfFile.find('\'') == string::npos && LineOfFile.find('-') == string::npos) //Palavra é válida se não contiver carateres especiais
					{
						vec.push_back(LineOfFile);
						numberWords++;
						break;
					}
					else
						break;
				}
				else //Se a linha ainda tiver ';' executa este pedaço de código
				{
					substrOfLine = LineOfFile.substr(0, LineOfFile.find(';')); //substrOfLine passa a ser a primeira palavra da linha
					if (substrOfLine.find(' ') == string::npos && substrOfLine.find('\'') == string::npos && substrOfLine.find('-') == string::npos) //Caso a substring só tenha carateres maiúsculos
					{
						vec.push_back(substrOfLine);
						numberWords++;
						LineOfFile = LineOfFile.substr(LineOfFile.find(';') + 2);
					}
					else if (substrOfLine.at(substrOfLine.length() - 1) == ' ') //Há um caso especial em que a linha a palavra tem um espaço antes do ';' , que é tratado aqui
					{
						vec.push_back(substrOfLine.substr(0, substrOfLine.find(' ')));
						numberWords++;
						LineOfFile = LineOfFile.substr(LineOfFile.find(';') + 2);
					}
					else if (LineOfFile.at(LineOfFile.length() - 1) == ';') //Trata outro caso especial em que a linha termina em ';'
							LineOfFile.at(LineOfFile.length() - 1) = '\n';	
					else //Qualquer outro caso (palavras inválidas em linhas com várias palavras)
						LineOfFile = LineOfFile.substr(LineOfFile.find(';') + 2);
				}
			}

			if (numberWords >= 100) //Ponto é mostrado a cada 100 palavras
			{
				cout << ".";
				numberWords = numberWords - 100;
			}
		}
	}
}


//Função que copia os valores do vetor que contém todas as palavras para o ficheiro selecionado pelo utilizador
void save_words(const vector <string> &vec, ofstream &file)
{
	for (int i = 0; i < vec.size(); i++)
	{
		file << vec.at(i) << endl;
	}
}

//Função que verifica se uma linha é válida. Isto é, verifica se a linha apenas tem apóstrofos, maiúsculas, hífens, espaços e ';'
bool validLine(string line)
{
	int position = 0; //Variável que dá a posição na linha
	bool valid = 0; //Variável que se torna 1 caso a linha seja válida

	while (position < line.length())
	{
		if ((line.at(position)>='A' && line.at(position) <= 'Z')|| line.at(position) == '-' || line.at(position) == ' ' || line.at(position) == '\'' || line.at(position) == ';')
		{
			valid = 1;
			position++;
		}
		else
		{
			valid = 0;
			break;
		}
	}

	return valid;
}