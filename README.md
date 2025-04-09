
# Projeto de Controlo de Trotinetes Elétricas 🛴🔋

Este projeto consiste na simulação de um sistema local de controlo de cacifos para trotinetes elétricas, com funcionalidades como **estacionamento**, **aluguer**, **gestão do estado de carregamento** e **registo de eventos com marca temporal**. 

Foi desenvolvido em **linguagem C**, no âmbito da unidade curricular de **Projeto de Controlo de um Equipamento (PCE)** da ESTSetúbal – Instituto Politécnico de Setúbal.

---

## 🎯 Objetivos do Projeto

- Controlar localmente o estado de várias trotinetes elétricas
- Simular **carregamento automático gradual**
- Simular **descarregamento quando a trotinete está em uso**
- Garantir **persistência de dados** através de ficheiros JSON
- Registar eventos no sistema com **data e hora**
- Prever integração futura com microcontroladores (Arduino / ESP32)

---

## ✅ Funcionalidades Principais

### 🧭 Interface em Terminal

O programa apresenta um menu interativo que permite:

- Estacionar uma trotinete
- Alugar uma trotinete
- Listar o estado de todas as trotinetes
- Sair do sistema

### 🔋 Carregamento automático simulado

- Quando uma trotinete é **estacionada**, o sistema:
  - Simula o carregamento da bateria **de 10 em 10%**
  - Informa o utilizador apenas de que o carregamento foi iniciado com a percentagem atual
  - **Regista todos os passos no log**, até atingir 100%
  - Garante que a bateria não ultrapassa esse valor

```plaintext
Trotinete 3 em carregamento... (50%)
```

### 🔻 Descarregamento automático

- Sempre que uma trotinete está **fora do estacionamento**, simula-se uma perda de 5% de bateria a cada interação.
- A bateria nunca desce abaixo de 0%.
- Todo o processo é também **registado no ficheiro de log**.

### 🧠 Validação de entrada

- O sistema valida se o número da trotinete é um número inteiro válido.
- Permite cancelar operações introduzindo **"9"**, voltando ao menu principal.

### 📁 Persistência de Dados

- O estado atual das trotinetes é guardado no ficheiro `estado_trotinetes.json`, garantindo que os dados persistem entre sessões.
- O programa **detecta automaticamente** quantas trotinetes existem.

### 📝 Registo de Eventos (Logs)

- Todos os eventos são guardados em `logs.txt`, com data e hora:
  - Estacionamento
  - Aluguer
  - Carregamento passo a passo
  - Descarregamento
  - Carregamento concluído

---

## 📦 Ficheiros do Projeto

| Ficheiro                 | Descrição                                           |
|--------------------------|-----------------------------------------------------|
| `main.c`                 | Código-fonte principal em linguagem C              |
| `estado_trotinetes.json` | Ficheiro com o estado atual de cada trotinete      |
| `logs.txt`               | Ficheiro de log de eventos com data e hora         |

---

## 💻 Exemplo de Execução

```plaintext
--- Menu ---
1. Estacionar trotinete
2. Alugar trotinete
3. Listar trotinetes
4. Sair
Escolha uma opção: 1

--- Lista de Trotinetes ---
Scooter Nº: 3
Nível de bateria: 50%
Estacionada: Não

Digite o número da trotinete: 3
Trotinete 3 estacionada com sucesso!
Trotinete 3 em carregamento... (50%)
```

---

## 🧾 Exemplo de Conteúdo de `logs.txt`

```plaintext
10-04-2025 14:22:30 - Trotinete 3 - Estacionada
10-04-2025 14:22:30 - Trotinete 3 - Carregamento: 50% → 60%
10-04-2025 14:22:30 - Trotinete 3 - Carregamento: 60% → 70%
10-04-2025 14:22:30 - Trotinete 3 - Carregamento: 90% → 100%
10-04-2025 14:22:30 - Trotinete 3 - Carregamento concluído
```

---

## 📂 Exemplo de Conteúdo de `estado_trotinetes.json`

```json
[
  {
    "numeroTrotinete": 1,
    "nivelBateria": 100,
    "estadoEstacionamento": true
  },
  {
    "numeroTrotinete": 2,
    "nivelBateria": 95,
    "estadoEstacionamento": false
  },
  {
    "numeroTrotinete": 3,
    "nivelBateria": 100,
    "estadoEstacionamento": true
  },
  {
    "numeroTrotinete": 4,
    "nivelBateria": 20,
    "estadoEstacionamento": false
  }
]
```

---

## 🚀 Considerações Finais

Este projeto representa uma **base sólida** para a gestão de trotinetes elétricas com simulação de estados. A implementação modular e baseada em ficheiros permite escalabilidade e integração futura com **microcontroladores físicos**, como o **ESP32**, de forma simples e eficaz.
