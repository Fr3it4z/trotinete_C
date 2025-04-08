# Projeto de Controlo de Trotinetes Elétricas 🛴🔋

Este projeto consiste na simulação do controlo de um equipamento com cacifos para trotinetes elétricas. Permite ao utilizador executar operações como **estacionar**, **alugar**, **consultar o estado** e acompanhar o **carregamento da bateria** de cada trotinete.

O desenvolvimento foi realizado em **linguagem C**, no contexto da unidade curricular de **Projeto de Controlo de um Equipamento (PCE)** da ESTSetúbal – Instituto Politécnico de Setúbal.

---

## 🎯 Objetivos do projeto

- Controlar localmente um sistema de cacifos para trotinetes
- Manter persistência do estado entre execuções
- Simular o processo de **carregamento automático da bateria**
- Registar eventos com **data e hora**
- Servir como base de integração com **Arduino e Nexys2**

---

## ✅ Funcionalidades principais

### 🧭 Interface no terminal

- Menu com opções de:
  - Estacionar uma trotinete
  - Alugar uma trotinete
  - Listar o estado das trotinetes
  - Sair do programa

### 🔋 Simulação de carregamento

- Ao estacionar uma trotinete:
  - A bateria é carregada automaticamente de 10 em 10%
  - É apresentada uma **barra de progresso no terminal**
  - Cada etapa é registada no log
  - A carga é limitada a 100%

### 📁 Persistência de dados (ficheiro JSON)

- O estado das trotinetes é guardado em `estado_trotinetes.json`:
  - Número
  - Nível de bateria
  - Estado (estacionada ou não)
- O programa deteta automaticamente o número total de trotinetes

### 📝 Registo de eventos (ficheiro de log)

- O ficheiro `logs.txt` é atualizado automaticamente com:
  - Estacionamento
  - Aluguer
  - Início e progresso de carregamento
  - Carregamento concluído
- Cada linha inclui data e hora do evento

---

## 📦 Ficheiros do projeto

| Ficheiro                 | Descrição                                     |
| ------------------------ | --------------------------------------------- |
| `main.c`                 | Código-fonte principal do programa            |
| `estado_trotinetes.json` | Ficheiro de dados com o estado das trotinetes |
| `logs.txt`               | Registo de eventos com data e hora            |

---

## 💻 Exemplo de execução no terminal

```plaintext
--- Menu ---
1. Estacionar trotinete
2. Alugar trotinete
3. Listar trotinetes
4. Sair
Escolha uma opção: 1

--- Lista de Trotinetes ---
Scooter Nº: 1
Nível de bateria: 40%
Estacionada: Não

Digite o número da trotinete: 1
Trotinete 1 estacionada com sucesso!
⏳ A carregar Trotinete 1...
🔋 Bateria: [████      ] 40%
🔋 Bateria: [█████     ] 50%
🔋 Bateria: [██████    ] 60%
🔋 Bateria: [███████   ] 70%
🔋 Bateria: [████████  ] 80%
🔋 Bateria: [█████████ ] 90%
🔋 Bateria: [██████████] 100%
✅ Trotinete 1 totalmente carregada!

🧾 Exemplo de conteúdo do ficheiro logs.txt

09-04-2025 16:30:05 - Trotinete 1 - Estacionada
09-04-2025 16:30:06 - Trotinete 1 - Carregamento: 40% → 50%
09-04-2025 16:30:07 - Trotinete 1 - Carregamento: 50% → 60%
09-04-2025 16:30:11 - Trotinete 1 - Carregamento: 90% → 100%
09-04-2025 16:30:11 - Trotinete 1 - Carregamento concluído

📂 Exemplo de conteúdo do ficheiro estado_trotinetes.json

[
  {
    "numeroTrotinete": 1,
    "nivelBateria": 100,
    "estadoEstacionamento": true
  },
  {
    "numeroTrotinete": 2,
    "nivelBateria": 100,
    "estadoEstacionamento": true
  },
  {
    "numeroTrotinete": 3,
    "nivelBateria": 50,
    "estadoEstacionamento": false
  },
  {
    "numeroTrotinete": 4,
    "nivelBateria": 20,
    "estadoEstacionamento": false
  }
]
```
