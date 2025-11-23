# PDI — Processamento Digital de Imagens (Qt/C++)

Projeto exemplo em C++ usando Qt (widgets) e CMake para estudos e experimentos em processamento digital de imagens.

## Visão geral

Este repositório contém uma aplicação desktop (Qt) para operações básicas de processamento de imagem. A interface gráfica está em `src/mainwindow.ui` e a lógica em `src/`.

## Estrutura do repositório

- `src/` — código-fonte C++ e arquivos .ui
- `include/` — headers públicos do projeto
- `images/` — imagens de exemplo (`in.jpg`, `out.jpg`, `no-image.png`)
- `resources.qrc` — recursos Qt
- `CMakeLists.txt` — instruções de build

## Requisitos

- Linux (testado)
- CMake >= 3.16
- Compilador C++ (g++/clang++)
- Qt (versão compatível com os arquivos `.ui` usados; ex.: Qt 5.12+ ou Qt 6)

## Build

No diretório do projeto execute os comandos abaixo (modo terminal):

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

Também há uma task do VS Code chamada "CMake build clean" configurada no workspace.

## Executar

Após o build, execute o binário gerado (por exemplo `build/PDI`):

```bash
./build/PDI
```

A aplicação abrirá a janela principal onde é possível carregar imagens (veja a pasta `images/` para exemplos).

## Uso básico

- Abra a aplicação
- Carregue uma imagem
- Aplique transformações/dispositivos disponíveis na interface
- Exporte/salve o resultado (conforme implementado)

## Contribuição

Issues e pull requests são bem-vindos. Para alterações significativas, abra uma issue descrevendo o objetivo.

## Licença

Adicione uma licença apropriada (ex.: MIT) criando um arquivo `LICENSE` na raiz do projeto.


---

Gerado automaticamente — edite conforme necessário para detalhar funcionalidades específicas do projeto.
