# Projeto de Controlo de Trotinetes Elétricas 🛴🔋

Este projeto consiste na simulação do controlo de um equipamento com cacifos para trotinetes elétricas, que permite ao utilizador realizar operações como **estacionar**, **alugar**, **consultar estado**, e acompanhar o **carregamento da bateria**.

Foi desenvolvido em linguagem **C**, no contexto da unidade curricular de **Projeto de Controlo de um Equipamento (PCE)** da ESTSetúbal – IPS.

---

## 🎯 Objetivo do projeto

- Controlar localmente um sistema de 4 cacifos para trotinetes
- Garantir persistência de estado entre execuções
- Simular o processo de **carregamento automático**
- Registar eventos com marca temporal
- Servir de base para a ligação ao Arduino e Nexys2 (futuramente)

---

## ✅ Funcionalidades principais

### 🧭 Interface no terminal

- Menu com as seguintes opções:
  - Estacionar uma trotinete
  - Alugar uma trotinete
  - Listar o estado de todas as trotinetes
  - Sair do programa

### 🔋 Simulação de carregamento

- Sempre que uma trotinete é estacionada:
  - A bateria aumenta automaticamente de 10 em 10%
  - É apresentada uma **barra de progresso no terminal**
  - O processo é **registado no log**
  - A bateria nunca ultrapassa os 100%

### 📁 Persistência de dados

- Utiliza um ficheiro `JSON` para guardar o estado das trotinetes:
  - Número
  - Nível de bateria
  - Estado (estacionada ou não)
- Permite expandir o número de trotinetes sem alterar o código

### 📝 Registo de eventos (logs)

- Criação automática de ficheiro `logs.txt`
- Inclui eventos com data e hora:
  - Estacionamento
  - Aluguer
  - Carregamento (passo a passo)
  - Carregamento concluído

---

## 📦 Ficheiros do projeto

| Ficheiro                 | Descrição                                           |
| ------------------------ | --------------------------------------------------- |
| `main.c`                 | Código-fonte principal do programa                  |
| `estado_trotinetes.json` | Ficheiro de dados com o estado atual das trotinetes |
| `logs.txt`               | Ficheiro de log com os eventos registados           |

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
```

## 📄 Exemplo de ficheiro logs.txt

09-04-2025 16:30:05 - Trotinete 1 - Estacionada
09-04-2025 16:30:06 - Trotinete 1 - Carregamento: 40% → 50%
09-04-2025 16:30:07 - Trotinete 1 - Carregamento: 50% → 60%
09-04-2025 16:30:11 - Trotinete 1 - Carregamento: 90% → 100%
09-04-2025 16:30:11 - Trotinete 1 - Carregamento concluído

## 📄 Exemplo de ficheiro logs.txt

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
