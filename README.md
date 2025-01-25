# Sistema de Controle de Registros de Atletas

## Colaboradores
- **Fábio Damas Valim**  
- **Guilherme Lírio Miranda**  
- **Marcos Vinícius Pereira**

---

## Introdução

Este relatório aborda a análise e o desempenho de um sistema projetado para o controle de registros de atletas. Para a manutenção de dados, utilizamos a prática de **arquivos binários**.  
As operações realizadas pelo sistema incluem: **inserção**, **busca**, **remoção** e **exibição** de registros. As informações são organizadas em um sistema de blocos, que utiliza estruturas específicas para os atletas e os cabeçalhos dos blocos.  

---

## Estruturas de Dados Utilizadas

### Estrutura Atleta
A estrutura `atleta` é responsável por armazenar as informações de cada atleta. Seus campos incluem:  
- **id**: Identificador único do atleta (inteiro).  
- **nome**: Nome do atleta (`char[150]`).  
- **sexo**: Sexo do atleta (`M` ou `F`).  
- **idade**: Idade do atleta (float).  
- **altura**: Altura do atleta (float).  
- **peso**: Peso do atleta (float).  
- **time**: Nome do time ao qual o atleta pertence (`char[150]`).  

### Estrutura Cabeçalho
A estrutura `cabeçalho` gerencia os blocos de dados no arquivo binário. Seus campos incluem:  
- **quantidade**: Número de registros no bloco.  
- **próximo**: Índice do próximo bloco (ou `-1` se não houver outro bloco).  

---

## Uso das Estruturas no Código

### Funções relacionadas à Estrutura Atleta
- **`escreverNoBloco()`**: Grava os dados de um atleta no arquivo binário.  
- **`buscarRegistro()`**: Localiza e exibe os dados de um atleta.  
- **`carregarArquivo()`**: Preenche os campos da estrutura a partir de um arquivo CSV.  
- **`removerRegistro()`**: Remove e reorganiza os registros no arquivo binário.  

### Funções relacionadas à Estrutura Cabeçalho
- **`lerCabecalho()`** e **`atualizarCabecalho()`**: Leitura e atualização do cabeçalho do bloco.  
- **`criarBloco()`**: Cria um novo bloco no arquivo binário.  
- **`dividirBloco()`**: Divide blocos para reorganizar os dados e manter a integridade.  

---

## Operações Realizadas pelo Programa

### Inserção de Dados
1. **Via Arquivo CSV**: Importação de dados mantêm a ordem dos IDs e dividem blocos automaticamente.  
2. **Manual**: Inserção manual respeita as mesmas regras de organização dos IDs.  

### Remoção de Dados
- Realiza a remoção de atletas pelo ID. Após a confirmação, reorganiza os registros e atualiza o cabeçalho.  

### Busca de Dados
- Busca sequencial pelos blocos até localizar o atleta ou determinar que o ID não existe.  

### Exibição de Dados
- Exibe todos os registros cadastrados, percorrendo todos os blocos.  

---

## Gerenciamento de Blocos e Cabeçalhos

### Divisão de Blocos
Quando um bloco atinge o limite de 50 registros, um novo bloco é criado e parte dos registros é transferida para ele.  

### Ordenação dos Registros
Os registros em cada bloco são ordenados pelo ID. A ordenação garante organização e facilita as operações de busca.  

### Gestão dos Cabeçalhos
Cada bloco contém:
- **Quantidade de registros.**  
- **Índice do próximo bloco na sequência.**  

Essas informações são atualizadas conforme novos dados são adicionados ou removidos.  

---

## Conclusão

Este projeto demonstrou como arquivos binários podem ser utilizados para gerenciar registros de maneira eficiente. Operações como inserção, remoção e busca foram desenvolvidas para garantir a integridade e organização dos dados.  

Apesar dos desafios enfrentados, como a implementação da lógica de divisão de blocos, a colaboração entre os membros foi essencial para o sucesso do projeto. A experiência prática proporcionada foi enriquecedora e reforçou nossa preparação para desafios na área de computação.  
