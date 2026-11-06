# Simulación de Elecciones Presidenciales en México
## Modelo Basado en Agentes con Monte Carlo e Interacción Social

Simulación computacional que combina **Modelado Basado en Agentes (ABM)**, **Monte Carlo** y **dinámicas de opinión** para estudiar cómo evolucionan las preferencias electorales durante una campaña presidencial en México, analizando la robustez de los pronósticos frente a la incertidumbre en las interacciones sociales.

---

## Metodología

### 1. Modelado Basado en Agentes (ABM)

Cada votante es un agente con atributos sociodemográficos (educación, edad, vivienda, ideología), una preferencia política inicial y un **entorno social** (familiares, amigos, vecinos, redes digitales). Las decisiones individuales se actualizan mediante reglas de influencia social, exposición a eventos de campaña y voto estratégico. El ABM captura heterogeneidad individual y efectos emergentes que los modelos agregados no observan.

### 2. Simulación de Monte Carlo

Se realizan \(N\) ejecuciones independientes muestreando parámetros inciertos (pesos de influencia, volatilidad, abstención, tamaño de redes). El estimador Monte Carlo de una cantidad de interés es:

$$
\hat{f}_N = \frac{1}{N} \sum_{k=1}^{N} f(\theta_k), \quad \theta_k \sim p(\theta)
$$

| Variable | Significado |
|---|---|
| \(N\) | Número de ejecuciones Monte Carlo (escenarios simulados) |
| \(\theta_k\) | Vector de parámetros del modelo en la ejecución \(k\) |
| \(p(\theta)\) | Distribución de probabilidad de los parámetros |
| \(f(\theta_k)\) | Cantidad de interés evaluada en el escenario \(k\) (p. ej., cuota de voto final) |
| \(\hat{f}_N\) | Estimador Monte Carlo de \(f\) (promedio sobre los \(N\) escenarios) |

El error estándar del estimador decrece como \(\text{SE}(\hat{f}_N) = \sigma_f / \sqrt{N}\), lo que justifica usar un \(N\) elevado para reducir la incertidumbre. Cada ejecución representa un escenario electoral posible.

### 3. Niveles de Agregación

El modelo opera en dos niveles: **individual** (cada agente decide según sus atributos y entorno) y **agregado** (los votos se agrupan por sección electoral, distrito, entidad y total nacional):

$$
V_{\text{agregado}} = \sum_{i \in \mathcal{G}} v_i
$$

| Variable | Significado |
|---|---|
| \(\mathcal{G}\) | Conjunto de agentes que pertenecen a la unidad territorial considerada |
| \(v_i\) | Indicador binario: \(1\) si el agente \(i\) votó por el candidato de interés, \(0\) en otro caso |
| \(V_{\text{agregado}}\) | Total de votos por el candidato en la unidad territorial |

Esto facilita la validación con datos del INE y la observación de patrones territoriales.

---

## Interacción Social y Cambio de Decisión

### Estructura del entorno social

Cada agente está inmerso en una red con **lazos fuertes** (familiares, amigos), **débiles** (vecinos, compañeros) y **digitales** (redes sociales). La red se construye con modelos **Barabási–Albert** (libre de escala) o **Watts–Strogatz** (mundo pequeño), calibrados con datos de redes sociales en México.

### Regla de confianza acotada (Bounded Confidence)

Un agente \(i\) solo es influenciado por \(j\) si su distancia ideológica es menor que un umbral de tolerancia:

$$
|x_i - x_j| < \epsilon
$$

| Variable | Significado |
|---|---|
| \(x_i\) | Posición ideológica del agente \(i\) (escala 0–10) |
| \(x_j\) | Posición ideológica del agente \(j\) (escala 0–10) |
| \(\epsilon\) | Umbral de tolerancia ideológica; si la distancia lo supera, \(i\) ignora a \(j\) (polarización) |

### Probabilidad de adopción de preferencia

$$
P(i \to j) = w_{ij} \cdot \frac{1}{1 + |x_i - x_j|} \cdot \mathbb{1}_{|x_i - x_j| < \epsilon}
$$

| Variable | Significado |
|---|---|
| \(P(i \to j)\) | Probabilidad de que el agente \(i\) adopte la preferencia de \(j\) |
| \(w_{ij}\) | Peso del lazo social entre \(i\) y \(j\) (fuerte, débil o digital) |
| \(1/(1 + \|x_i - x_j\|)\) | Penalización por distancia ideológica (a mayor distancia, menor peso) |
| \(\mathbb{1}_{\|x_i - x_j\| < \epsilon}\) | Indicador: vale \(1\) si se cumple la confianza acotada, \(0\) si no |

### Actualización de la posición ideológica

$$
x_i(t+1) = x_i(t) + \mu \cdot w_{ij} \cdot (x_j(t) - x_i(t))
$$

| Variable | Significado |
|---|---|
| \(x_i(t)\) | Posición ideológica del agente \(i\) en el paso \(t\) |
| \(x_j(t)\) | Posición ideológica del agente \(j\) (fuente de influencia) en el paso \(t\) |
| \(x_i(t+1)\) | Posición actualizada del agente \(i\) |
| \(\mu\) | Tasa de convergencia \(\in (0,1]\); \(\mu=1\) adopta totalmente la posición de \(j\), \(\mu \to 0\) apenas se mueve |
| \(w_{ij}\) | Peso del lazo social entre \(i\) y \(j\) |

### Factores que modulan la influencia

| Factor | Efecto |
|---|---|
| **Fuerza del lazo** | Lazos fuertes pesan más que débiles; digitales dependen del algoritmo. |
| **Distancia ideológica** | A mayor distancia, menor probabilidad de cambio. |
| **Educación** | Mayor educación → mayor resistencia a lazos débiles. |
| **Edad** | Jóvenes más influenciables por lazos digitales; adultos mayores por lazos fuertes. |
| **Vivienda** | Mejores condiciones → mayor exposición a opiniones diversas. |
| **Ideología** | Agentes extremos tienen umbrales \(\epsilon\) más pequeños (menos tolerancia). |

### Dinámica temporal (un día de campaña)

1. Cada agente selecciona aleatoriamente un contacto de su red.
2. Se evalúa la regla de confianza acotada: \(|x_i - x_j| < \epsilon\).
3. Si se cumple, se actualiza la posición con probabilidad \(P(i \to j)\).
4. Se registra el cambio (o no) en la intención de voto.

El proceso se repite hasta el día de la elección, generando una trayectoria de decisión por agente comparable con encuestas panel.

---

## Modelo Matemático del Voto (Hotelling-Downs)

Marco de competencia espacial reformulado desde la física estadística para calcular la cuota de voto de cada candidato según su posición.

### Cuota base (región de Voronoi)

$$
V = \iint_{\text{Voronoi}} W(\vec{x}) \, d\vec{x} \tag{1}
$$

| Variable | Significado |
|---|---|
| \(V\) | Cuota de voto del partido (sin efectos de abstención ni activistas) |
| \(W(\vec{x})\) | Densidad de votantes en la posición ideológica \(\vec{x}\) |
| \(\vec{x}\) | Vector de posición en el espacio ideológico |
| \(\text{Voronoi}\) | Región del espacio donde el partido es el más cercano al votante |

### Efecto de la abstención

$$
V_T = \int_{\text{Voronoi}} \frac{W(\vec{x})}{\left(1 + (\vec{x} - \vec{x_p})^2\right)^{\tau}} \, d\vec{x} \tag{2}
$$

| Variable | Significado |
|---|---|
| \(V_T\) | Cuota de voto ajustada por abstención |
| \(\vec{x_p}\) | Posición ideológica del partido |
| \(\tau\) | Propensión a la abstención por alienación; \(\tau \to 0\) recupera la ecuación base |
| \((\vec{x} - \vec{x_p})^2\) | Distancia cuadrática entre votante y partido |

### Efecto de los activistas (sesgo hacia extremos)

$$
V_{T,A} = \int_{\text{Voronoi}} \frac{W(\vec{x}) \, |\vec{x}|^{2\alpha}}{\left(1 + (\vec{x} - \vec{x_p})^2\right)^{\tau}} \, d\vec{x} \tag{3}
$$

| Variable | Significado |
|---|---|
| \(V_{T,A}\) | Cuota de voto ajustada por abstención **y** activistas |
| \(\alpha\) | Fuerza de la influencia de votantes extremos; \(\alpha > 0\) sesga hacia los polos |
| \(\|\vec{x}\|^{2\alpha}\) | Peso extra a votantes alejados del centro ideológico |

### Dinámica de los partidos: Algoritmo de Metropolis

Los partidos ajustan sus posiciones aceptando un nuevo vector de políticas con probabilidad:

$$
P(V_1, V_2) = e^{(V_2 - V_1)/T}
$$

| Variable | Significado |
|---|---|
| \(P(V_1, V_2)\) | Probabilidad de aceptar el cambio de posición |
| \(V_1\) | Cuota de voto con la posición actual |
| \(V_2\) | Cuota de voto con la nueva posición propuesta |
| \(T\) | Temperatura (volatilidad); \(T\) alto permite aceptar cambios que reducen la cuota |

---

## Sociodemografía de los Agentes

Los agentes se inicializan con atributos calibrados con datos del INEGI y encuestas nacionales:

| Atributo | Descripción | Valores / Escala |
|---|---|---|
| **Educación** | Nivel educativo máximo | Primaria, Secundaria, Bachillerato, Licenciatura, Posgrado |
| **Edad** | Años cumplidos | 18-29, 30-44, 45-59, 60+ |
| **Vivienda** | Índice compuesto de condiciones | 0–1 |
| **Ideología** | Autoubicación izquierda-derecha | Continua 0–10 |

La probabilidad inicial de voto por el candidato \(c\) sigue una función logística (softmax):

$$
P(v_i = c) = \frac{\exp\left(\beta_c^\top z_i\right)}{\sum_{k \in \mathcal{C}} \exp\left(\beta_k^\top z_i\right)}
$$

| Variable | Significado |
|---|---|
| \(P(v_i = c)\) | Probabilidad de que el agente \(i\) vote por el candidato \(c\) |
| \(z_i\) | Vector de atributos sociodemográficos del agente \(i\) (educación, edad, vivienda, ideología) |
| \(\beta_c\) | Vector de coeficientes del candidato \(c\) (estimados con encuestas) |
| \(\beta_c^\top z_i\) | Producto punto: utilidad latente del candidato \(c\) para el agente \(i\) |
| \(\mathcal{C}\) | Conjunto de candidatos |

Estos atributos modulan además la susceptibilidad al cambio, el tamaño de la red social y el umbral de tolerancia \(\epsilon\).

---

## Sistema Electoral: Elecciones Presidenciales en México

El modelo se enfoca en la elección presidencial mexicana considerando:

- **Candidatos y partidos:** coaliciones y partidos vigentes.
- **Reglas:** mayoría relativa en una sola vuelta (sin segunda vuelta).
- **Territorio:** 300 distritos electorales, 32 entidades federativas.
- **Calendario:** campaña de aprox. 90 días con eventos clave (debates, cierres).

La regla de decisión final es:

$$
\text{Ganador} = \arg\max_{c \in \mathcal{C}} \; V_{\text{nacional}}(c)
$$

| Variable | Significado |
|---|---|
| \(\text{Ganador}\) | Candidato que obtiene la victoria |
| \(\mathcal{C}\) | Conjunto de candidatos |
| \(V_{\text{nacional}}(c)\) | Suma de votos simulados para el candidato \(c\) en todas las secciones electorales |
| \(\arg\max\) | Operador que devuelve el candidato con mayor \(V_{\text{nacional}}\) |

---

## Fuentes Consultadas

- Barabási, A.-L., Albert, R., & Jeong, H. (1999). Mean-field theory for scale-free random networks. *Physica A*, 272(1–2), 173–187.
- Watts, D. J., & Strogatz, S. H. (1998). Collective dynamics of 'small-world' networks. *Nature*, 393, 440–442.
- Hegselmann, R., & Krause, U. (2002). Opinion dynamics and bounded confidence models, analysis, and simulation. *Journal of Artificial Societies and Social Simulation*, 5(3).
- Deffuant, G., Neau, D., Amblard, F., & Weisbuch, G. (2000). Mixing beliefs among interacting agents. *Advances in Complex Systems*, 3, 87–98.
- Campbell, C., & Ackland, G. J. (2026). A computational model of spatial politics: Hotelling-downs model as statistical physics. *PLOS One*, 21(6), e0352242.
- Metropolis, N., Rosenbluth, A. W., Rosenbluth, M. N., Teller, A. H., & Teller, E. (1953). Equation of state calculations by fast computing machines. *The Journal of Chemical Physics*, 21(6), 1087–1092.
- Agent-based Simulation of District-based Elections. (2023). *arXiv*.
- Instituto Nacional Electoral (INE). (2024). *Estudio Muestral de Participación Ciudadana 2024*.
- Instituto Nacional de Estadística y Geografía (INEGI). (2025). *Encuesta Intercensal 2025*.

---

**Nota:** Las ecuaciones y conceptos presentados sintetizan los marcos teóricos de las fuentes anteriores y se adaptan al contexto específico de la elección presidencial mexicana.
