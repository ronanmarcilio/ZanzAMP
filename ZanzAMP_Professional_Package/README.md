# ZanzAMP - VST3 Plugin

O **ZanzAMP** é um plugin VST3 inspirado no clássico SansAmp PSA-1, focado em simulação de amplificadores e distorção analógica.

## Controles
- **PRE-AMP**: Ganho de entrada.
- **BUZZ**: Saturação de frequências baixas.
- **PUNCH**: Overdrive de médios.
- **CRUNCH**: Harmônicos de frequências altas.
- **DRIVE**: Simulação de distorção de power amp.
- **LOW/HIGH**: Ajustes finos de equalização.
- **LEVEL**: Volume de saída.

## Como Compilar (Windows)

1. **Pré-requisitos**:
   - Instale o [Visual Studio 2022](https://visualstudio.microsoft.com/) (com suporte a C++).
   - Instale o [CMake](https://cmake.org/).
   - Baixe o framework [JUCE](https://juce.com/download/).

2. **Configuração**:
   - Coloque a pasta do JUCE dentro do diretório do projeto ZanzAMP ou ajuste o caminho no `CMakeLists.txt`.
   - Abra o terminal na pasta do projeto.

3. **Build**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

4. **Instalação**:
   - O arquivo `ZanzAMP.vst3` será gerado na pasta `build/ZanzAMP_artefacts/Release/VST3`.
   - Copie-o para a pasta de plugins da sua DAW (geralmente `C:\Program Files\Common Files\VST3`).

## Estrutura do Projeto
- `Source/`: Contém o código-fonte C++.
- `CMakeLists.txt`: Arquivo de configuração para o build.
- `README.md`: Este guia.
