# ZanzAMP - Documentação Técnica e Guia de Build

Este documento contém as instruções para compilar o plugin **ZanzAMP** e gerar o instalador `.exe` conforme as especificações profissionais solicitadas.

## Especificações DSP Implementadas
- **Oversampling 4x**: Processamento interno em alta taxa de amostragem para evitar aliasing na distorção.
- **Parameter Smoothing**: Transições suaves em todos os knobs para evitar cliques.
- **DC Offset Removal**: Filtro High-Pass em 20Hz após o estágio de saturação.
- **Multi-stage Saturation**: Algoritmos de distorção baseados em `tanh`, `sin` e `atan` para harmônicos complexos.
- **True Bypass**: Implementado via parâmetro automável.

## Estrutura de Arquivos
- `/Source`: Código C++ (JUCE).
- `/Installer`: Script `.iss` para o Inno Setup.
- `CMakeLists.txt`: Configuração do projeto para Visual Studio 2022.

## Passo a Passo para Gerar o Instalador (.exe)

### 1. Compilar o Plugin (VST3)
1. No Windows, instale o **Visual Studio 2022** e o **CMake**.
2. Baixe o framework **JUCE** e coloque a pasta `JUCE` dentro do diretório do projeto.
3. Abra o terminal na pasta do projeto e execute:
   ```bash
   mkdir build
   cd build
   cmake .. -G "Visual Studio 17 2022" -A x64
   cmake --build . --config Release
   ```
4. Isso gerará o arquivo `ZanzAMP.vst3` em `build/ZanzAMP_artefacts/Release/VST3/`.

### 2. Gerar o Instalador (.exe)
1. Instale o [Inno Setup](https://jrsoftware.org/isdl.php).
2. Vá até a pasta `/Installer` e abra o arquivo `ZanzAMP_Installer.iss`.
3. Clique em **Compile** (ou aperte F9).
4. O instalador `Setup_ZanzAMP.exe` será criado na pasta `build/Installer/`.

## Local de Instalação Padrão
O instalador colocará o plugin automaticamente em:
`C:\Program Files\Common Files\VST3\ZanzAudio\ZanzAMP.vst3`

---
**Desenvolvido por ZanzAudio**
