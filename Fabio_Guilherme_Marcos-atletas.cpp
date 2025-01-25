/* ======================================================== *                                                              
    
    EQUIPE:
        - Fábio Damas Valim (202410372)
        - Guilherme Lirio Miranda (202410367)
        - Marcos Vinicius Pereira (202411098)

* =========================================================

        -> Inserção de dados via arquivo texto (.csv)
        -> Inserção de dado via entrada padrão
        -> Remoção de dado específico
        -> Busca de dado específico

ULTIMA ATUALIZACAO: 24/01/2025 - 10:00 am

* ========================================================= */

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>

using namespace std;

struct atleta{
    int id;
    char nome[150];
    char sexo;
    float idade;
    float altura;
    float peso;
    char time[150];
};

struct cabecalho {
    int quantidade;
    int proximo;
};

cabecalho lerCabecalho(int indice) {
    string arquivo = to_string(indice) + ".dat";

    cabecalho meuCabecalho;

    fstream lerBin(arquivo, ios::in | ios::out | ios::binary);

    lerBin.seekg(0);
    lerBin.read(reinterpret_cast<char*>(&meuCabecalho), sizeof(cabecalho));
    lerBin.close();
    
    return meuCabecalho;
}

void atualizarCabecalho(int indice, cabecalho meuCabecalho) {
    string arquivo = to_string(indice) + ".dat";

    // passa como parametro o cabecalho atualizado e escreve no arquivo
    fstream gravaBin(arquivo, ios::in | ios::out | ios::binary);

    gravaBin.seekp(0);
    gravaBin.write(reinterpret_cast<char*>(&meuCabecalho), sizeof(cabecalho));
    gravaBin.close();
}

void criarBloco(int indice) {
    cabecalho meuCabecalho;
    meuCabecalho.quantidade = 0;
    meuCabecalho.proximo = -1; // Inicialmente aponta para -1

    string arquivo = to_string(indice) + ".dat";

    // Criar o arquivo em modo de escrita para garantir sua criação
    fstream gravaBin(arquivo, ios::out | ios::binary);
    if (!gravaBin) {
        cerr << "Erro ao criar o arquivo " << arquivo << endl;
        return;
    }

    // Escrever o cabeçalho inicial no arquivo
    gravaBin.write(reinterpret_cast<char*>(&meuCabecalho), sizeof(cabecalho));
    gravaBin.close();
}

void ordenarRegistros(int indice, int quantidadeRegistros) {
    string arquivo = to_string(indice) + ".dat";

    // Declarar um vetor dinamicamente para armazenar os registros
    atleta* registros = new atleta[quantidadeRegistros];

    fstream arqBin(arquivo, ios::in | ios::out | ios::binary);

    if (!arqBin) {
        cerr << "Erro ao abrir o arquivo " << arquivo << endl;
        delete[] registros;
        return;
    }

    // Pular o cabeçalho do arquivo (assumindo que ele tenha tamanho fixo)
    int tamanhoCabecalho = sizeof(cabecalho);
    arqBin.seekg(tamanhoCabecalho, ios::beg);

    // Carregar os registros na memória
    for (int i = 0; i < quantidadeRegistros; i++) {
        arqBin.seekg(tamanhoCabecalho + i * sizeof(atleta), ios::beg); // Posição correta no arquivo
        arqBin.read(reinterpret_cast<char*>(&registros[i]), sizeof(atleta));
    }

    // Ordenar os registros na memória com base no ID
    for (int i = 0; i < quantidadeRegistros - 1; i++) {
        for (int j = 0; j < quantidadeRegistros - i - 1; j++) {
            if (registros[j].id > registros[j + 1].id) {
                // Troca os registros
                atleta temp = registros[j];
                registros[j] = registros[j + 1];
                registros[j + 1] = temp;
            }
        }
    }

    // Escrever os registros ordenados de volta no arquivo
    for (int i = 0; i < quantidadeRegistros; i++) {
        arqBin.seekp(tamanhoCabecalho + i * sizeof(atleta), ios::beg); // Posição correta no arquivo
        arqBin.write(reinterpret_cast<char*>(&registros[i]), sizeof(atleta));
    }

    // Liberar a memória alocada
    delete[] registros;
    arqBin.close();
}

void escreverNoBloco(int indice, atleta& novoRegistro) {
    string arquivo = to_string(indice) + ".dat";

    cabecalho meuCabecalho = lerCabecalho(indice);
    int quantidadeRegistros = meuCabecalho.quantidade;

    fstream gravaBin(arquivo, ios::in | ios::out | ios::binary);
    
    gravaBin.seekp(sizeof(cabecalho) + quantidadeRegistros * sizeof(atleta));
    gravaBin.write(reinterpret_cast<char*>(&novoRegistro), sizeof(atleta));

    gravaBin.close();
}

int espiaId(int indice) {
    atleta meuRegistro;
    string arquivo = to_string(indice) + ".dat";

    fstream lerBin(arquivo, ios::in | ios::out | ios::binary);

    lerBin.seekg(sizeof(cabecalho), ios::beg);
    lerBin.read(reinterpret_cast<char*>(&meuRegistro), sizeof(atleta));
    lerBin.close();

    return meuRegistro.id;
}

int verificaMaiorId(int indice) {
    //verifica maior id quando o bloco estiver cheio para saber se devera divir ou nao
    //como eu sei que ele esta cheio basta olhar a ultima posicao
    atleta meuRegistro;
    string arquivo = to_string(indice) + ".dat";
    fstream arqBin(arquivo, ios::in | ios::out | ios::binary);

    arqBin.seekg(sizeof(cabecalho) + (49 * sizeof(atleta))); // Posição correta no arquivo
    arqBin.read(reinterpret_cast<char*>(&meuRegistro), sizeof(atleta));

    return meuRegistro.id;
}

int proximoLivre(int indice) {
    int proximoBlocoLivre;
    bool verifica = false;
    
    while(!verifica) {
        cabecalho meuCabecalho = lerCabecalho(indice);

        if(meuCabecalho.proximo == -1) { //este e o ultimo bloco
            proximoBlocoLivre = (indice + 1);
            verifica = true;
        } else {
            indice = meuCabecalho.proximo;
        }
    }

    return proximoBlocoLivre;
}

void dividirBloco(int indice) {
    string blocoAntigoNome = to_string(indice) + ".dat";
    cabecalho cabecalhoAntigo = lerCabecalho(indice);

    // Determinar o próximo bloco livre
    int blocoLivre = proximoLivre(indice);

    criarBloco(blocoLivre);
    cabecalho novoCabecalho = lerCabecalho(blocoLivre);
    
    novoCabecalho.proximo = cabecalhoAntigo.proximo;
    novoCabecalho.quantidade = 0;
    atualizarCabecalho(blocoLivre, novoCabecalho);

    //ler os 26 maiores blocos
    atleta* registros = new atleta[26];

    fstream arqBin(blocoAntigoNome, ios::in | ios::out | ios::binary);

    // Pular o cabeçalho do arquivo (assumindo que ele tenha tamanho fixo)
    int tamanhoCabecalho = sizeof(cabecalho);
    arqBin.seekg(tamanhoCabecalho, ios::beg);

    // Carregar os 26 maiores registros na memória
    for (int i = 25; i < 51; i++) {
        arqBin.seekg(tamanhoCabecalho + i * sizeof(atleta), ios::beg); // Posição correta no arquivo
        arqBin.read(reinterpret_cast<char*>(&registros[i - 25]), sizeof(atleta));
    }

    arqBin.close();

    cabecalhoAntigo.proximo = blocoLivre;
    cabecalhoAntigo.quantidade = 25;
    atualizarCabecalho(indice, cabecalhoAntigo);


    //passar os 26 registros maiores para o novo arquivo
    for(int i = 0; i < 26; i++) {
        escreverNoBloco(blocoLivre, registros[i]);
        novoCabecalho.quantidade++;
        atualizarCabecalho(blocoLivre, novoCabecalho);
    }
}

void inserirRegistro(atleta& novoRegistro) {
    //verificar em qual bloco ele devera ser inserido
    //comparar elemento com o primeiro do proximo arquivo

    int indice = 0;
    bool verifica = false;

    while(!verifica) {
        cabecalho meuCabecalho = lerCabecalho(indice);

        if(meuCabecalho.proximo == -1 and meuCabecalho.quantidade < 50) {
            escreverNoBloco(indice, novoRegistro);
            meuCabecalho.quantidade++;
            ordenarRegistros(indice, meuCabecalho.quantidade);
            atualizarCabecalho(indice, meuCabecalho);

            return;
        }

        if(meuCabecalho.proximo == -1 and meuCabecalho.quantidade == 50 and 
        novoRegistro.id < verificaMaiorId(indice)) {
			escreverNoBloco(indice, novoRegistro);
            meuCabecalho.quantidade++;
            ordenarRegistros(indice, meuCabecalho.quantidade);
            atualizarCabecalho(indice, meuCabecalho);
            
            dividirBloco(indice);

            return;
        }

        int idProximo = espiaId(meuCabecalho.proximo);
        
        if(novoRegistro.id > verificaMaiorId(indice) and meuCabecalho.quantidade == 50
			and meuCabecalho.proximo == -1) {
            int blocoLivre = proximoLivre(indice);
            criarBloco(blocoLivre);
            cabecalho novoCabecalho = lerCabecalho(blocoLivre);
            
            escreverNoBloco(blocoLivre, novoRegistro);
            novoCabecalho.quantidade++;
            
            atualizarCabecalho(blocoLivre, novoCabecalho);

            meuCabecalho.proximo = blocoLivre;
            atualizarCabecalho(indice, meuCabecalho);
            return;
        }

        if(novoRegistro.id < idProximo and meuCabecalho.quantidade < 50
			and meuCabecalho.proximo != -1) {
            escreverNoBloco(indice, novoRegistro);
            meuCabecalho.quantidade++;
            ordenarRegistros(indice, meuCabecalho.quantidade);
            atualizarCabecalho(indice, meuCabecalho);

            return;
        }

        if(novoRegistro.id < idProximo and meuCabecalho.quantidade == 50) {
            int blocoLivre = proximoLivre(indice);
            criarBloco(blocoLivre);
            cabecalho novoCabecalho = lerCabecalho(blocoLivre);
            escreverNoBloco(blocoLivre, novoRegistro);
            novoCabecalho.quantidade++;
            ordenarRegistros(blocoLivre, novoCabecalho.quantidade);
            atualizarCabecalho(blocoLivre, novoCabecalho);

            meuCabecalho.proximo = blocoLivre;
            atualizarCabecalho(indice, meuCabecalho);
            return;
        }

        indice = meuCabecalho.proximo;
    }
}

void carregarArquivo() {
    string arq, linha, registroString;
    atleta meuRegistro;
    char lixo;

    cout << "Digite o nome do arquivo csv(com extensao): ";
    cin >> arq;

    ifstream arquivoCsv(arq);

    cin.ignore();
    getline(arquivoCsv, linha); // ignora o cabecalho

    while(arquivoCsv >> meuRegistro.id) {
        arquivoCsv >> lixo;

        // lendo o nome como uma string 
        // e transformando em um vetor de caracteres
        getline(arquivoCsv, registroString,','); 
        strcpy(meuRegistro.nome, registroString.c_str());

        arquivoCsv >> meuRegistro.sexo;
        arquivoCsv >> lixo;

        arquivoCsv >> meuRegistro.idade;
        arquivoCsv >> lixo;

        arquivoCsv >> meuRegistro.altura;
        arquivoCsv >> lixo;

        arquivoCsv >> meuRegistro.peso;
        arquivoCsv >> lixo;

        getline(arquivoCsv,registroString);
        strcpy(meuRegistro.time, registroString.c_str());

        inserirRegistro(meuRegistro);
    }

    cout << "\n*  Arquivo carregado com sucesso!  *\n" << endl;
}

atleta buscaId(int idProcurado) {
    atleta registroProcurado;
    int indice = 0;
    int quantidade = 0;

    registroProcurado.id = -1;

    while(indice != -1) {
        string arquivo = to_string(indice) + ".dat";
        fstream arqBin(arquivo, ios::in | ios::out | ios::binary);

        cabecalho meuCabecalho = lerCabecalho(indice);
        quantidade = meuCabecalho.quantidade;
        atleta meuRegistro;

        for(int i = 0; i < quantidade; i++) {
            arqBin.seekg(sizeof(cabecalho) + i * sizeof(atleta)); // Posição correta no arquivo
            arqBin.read(reinterpret_cast<char*>(&meuRegistro), sizeof(atleta));

            if(idProcurado == meuRegistro.id) {
                return meuRegistro;
            }
        }
    
        arqBin.close();
        indice = meuCabecalho.proximo;
    }

    return registroProcurado;
}

void buscarRegistro() {
    int id;

    cout << "\nDigite o id que deseja procurar: ";
    cin >> id;

    atleta registroProcurado = buscaId(id);

    if(registroProcurado.id == -1) {
        cout << "\n*  Registro NAO encontrado!  *\n" << endl;
    } else {
        cout << "\n*  Registro ENCONTRADO!  *\n" << endl;
        cout << "Id: " << registroProcurado.id
                << " - Nome: " << registroProcurado.nome
                << ", Sexo: " << registroProcurado.sexo
                << ", Idade: " << registroProcurado.idade
                << ", altura: " << registroProcurado.altura
                << ", Peso: " << registroProcurado.peso
                << ", Time: " << registroProcurado.time << endl;
    }
}

void inserir_entrada_padrao() {
    atleta meuRegistro;
    int tentativaId;

    cout << "\nDigite o ID: ";
    cin >> tentativaId;;

    if(buscaId(tentativaId).id != (-1)) {
        cout << "\n*  ERRO! Esse id ja existe!  *\n" << endl;
        return;
    } else {
        meuRegistro.id = tentativaId;
    }

    cout << "Digite o nome do atleta: ";
    cin.ignore();
    cin.getline(meuRegistro.nome, 150);

    cout << "Digite o sexo (M ou F): ";
    cin >> meuRegistro.sexo;

    cout << "Digite a idade: ";
    cin >> meuRegistro.idade;

    cout << "Digite a altura (cm): ";
    cin >> meuRegistro.altura;

    cout << "Digite o peso (Kg): ";
    cin >> meuRegistro.peso;

    cout << "Digite o time: ";
    cin.ignore();
    cin.getline(meuRegistro.time, 150);

    inserirRegistro(meuRegistro);

    cout << "\n*  Registro inserido com sucesso!  *\n" << endl;
}

void visualizar_dados() {
    int indice = 0;
    int quantidade = 0;

    while(indice != -1) {
        string arquivo = to_string(indice) + ".dat";
        fstream arqBin(arquivo, ios::in | ios::out | ios::binary);
        
        cout << "\n----------------------- BLOCO " << indice 
        << " ------------------------------------" << endl;

        cabecalho meuCabecalho = lerCabecalho(indice);
        quantidade = meuCabecalho.quantidade;
        atleta meuRegistro;

        for(int i = 0; i < quantidade; i++) {
            arqBin.seekg(sizeof(cabecalho) + i * sizeof(atleta)); // Posição correta no arquivo
            arqBin.read(reinterpret_cast<char*>(&meuRegistro), sizeof(atleta));

            cout << "Id: " << meuRegistro.id
                << " - Nome: " << meuRegistro.nome
                << ", Sexo: " << meuRegistro.sexo
                << ", Idade: " << meuRegistro.idade
                << ", altura: " << meuRegistro.altura
                << ", Peso: " << meuRegistro.peso
                << ", Time: " << meuRegistro.time << endl;
        }
   
        indice = meuCabecalho.proximo;
    }
}

void removerRegistro() {
    int idRemover;
    int indice = 0; // Índice do bloco inicial
    int indiceProcurado = -1; // Bloco onde o registro será encontrado
    int posicao = -1; // Posição do registro no bloco
	atleta armazenarRegistro;
	atleta meuRegistro;
	
    cout << "\nDigite o id que deseja excluir: ";
    cin >> idRemover;

    // Verifica se o ID existe
    if (buscaId(idRemover).id == -1) {
        cout << "\n*  ERRO! Esse id nao existe!  *\n" << endl;
        return;
    }

    // Procurar o bloco onde está o registro
    while (indice != -1) {
        string arquivo = to_string(indice) + ".dat";
        fstream arqBin(arquivo, ios::in | ios::binary);

        cabecalho meuCabecalho = lerCabecalho(indice);
        int quantidade = meuCabecalho.quantidade;

        for (int i = 0; i < quantidade; i++) {
            arqBin.seekg(sizeof(cabecalho) + i * sizeof(atleta), ios::beg);
            arqBin.read(reinterpret_cast<char*>(&meuRegistro), sizeof(atleta));

            if (idRemover == meuRegistro.id) {
				armazenarRegistro = meuRegistro;
                indiceProcurado = indice;
                posicao = i; // Posição no bloco
                i = quantidade; //encerrar repeticao
            }
        }

        indice = meuCabecalho.proximo; // Avançar para o próximo bloco
    }

    if (indiceProcurado == -1 and posicao == -1) {
        cout << "\n*  ERRO! Registro nao encontrado!  *\n" << endl;
        return;
    }
    
    cout << "\nTem certeza que deseja remover o registro: " << endl;
    cout << "Id: " << meuRegistro.id
                << " - Nome: " << meuRegistro.nome
                << ", Sexo: " << meuRegistro.sexo
                << ", Idade: " << meuRegistro.idade
                << ", altura: " << meuRegistro.altura
                << ", Peso: " << meuRegistro.peso
                << ", Time: " << meuRegistro.time << endl;
                
    int opcao = -1;
    
    cout << "\n1 - SIM" << endl << "2 - NAO" << endl;
    cin >> opcao;
    
    if(opcao == 1) {
		// Abrir o bloco onde o registro foi encontrado
		string arquivo = to_string(indiceProcurado) + ".dat";
		fstream arqBin(arquivo, ios::in | ios::out | ios::binary);

		cabecalho meuCabecalho = lerCabecalho(indiceProcurado);
		int quantidade = meuCabecalho.quantidade;

		// Carregar os registros na memória
		atleta* registros = new atleta[quantidade];
		for (int i = 0; i < quantidade; i++) {
			arqBin.seekg(sizeof(cabecalho) + i * sizeof(atleta), ios::beg);
			arqBin.read(reinterpret_cast<char*>(&registros[i]), sizeof(atleta));
		}

		// Remover o registro (deslocar os registros seguintes para a esquerda)
		for (int i = posicao; i < quantidade - 1; i++) {
			registros[i] = registros[i + 1];
		}

		// Atualizar o número de registros
		meuCabecalho.quantidade--;

		// Escrever os registros atualizados de volta no arquivo
		arqBin.seekp(sizeof(cabecalho), ios::beg);
		for (int i = 0; i < meuCabecalho.quantidade; i++) {
			arqBin.write(reinterpret_cast<char*>(&registros[i]), sizeof(atleta));
		}

		// Atualizar o cabeçalho no arquivo
		atualizarCabecalho(indiceProcurado, meuCabecalho);

		// Liberar memória e fechar arquivo
		delete[] registros;
		arqBin.close();

		cout << "\n*  Registro removido com sucesso!  *\n" << endl;
	} else {
		return;
	}
}

void menu() {
    cout << "\n*------------------- MENU -------------------*" << endl;
    cout << "|                                            |" << endl;
    cout << "| 0 - Sair                                   |" << endl;
    cout << "| 1 - Insercao de dados via arquivo texto    |" << endl;
    cout << "| 2 - Insercao de dados via entrada padrao   |" << endl;
    cout << "| 3 - Remocao de dado especifico             |" << endl;
    cout << "| 4 - Busca de dado especifico               |" << endl;
    cout << "| 5 - Visualizar dados                       |" << endl;
    cout << "|                                            |" << endl;
    cout << "*--------------------------------------------*" << endl;
}

int main() {
    int opcao = -1;
    criarBloco(0);

    while(opcao != 0) {
        menu();

        cout << "\nOpcao: ";

        cin >> opcao;

        switch(opcao) {
            case 1:
                carregarArquivo();
                break;
            case 2:
                inserir_entrada_padrao();
                break;
            case 3:
                removerRegistro();
                break;
            case 4:
                buscarRegistro();
                break;
            case 5:
                visualizar_dados();
                break;                
        }
    }

    cout << "\nFim do programa!\n" << endl;

    return 0;
}
