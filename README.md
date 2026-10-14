# Simulación de Elecciones Presidenciales en México
## Modelo Basado en Agentes con Monte Carlo e Interacción Social

Este proyecto implementa una simulación computacional para estudiar la evolución de las preferencias electorales durante una campaña presidencial en México. El modelo combina **Modelado Basado en Agentes (ABM)**, **simulación de Monte Carlo**, distintos **niveles de agregación** y un módulo explícito de **interacción social** que determina si un votante cambia o mantiene su decisión a lo largo de la campaña.

---

##  Metodología

### 1. Modelado Basado en Agentes (ABM)
Cada votante se representa como un **agente autónomo** con:
- Atributos sociodemográficos (educación, edad, vivienda, ideología).
- Preferencia política inicial (probabilidad de voto por cada candidato).
- **Un entorno social** compuesto por familiares, amigos, vecinos y contactos en redes sociales.
- Reglas de actualización de opinión basadas en:
  - **Interacciones con su entorno social** (ver Sección 4).
  - Exposición a eventos de campaña (debates, escándalos, anuncios).
  - Voto estratégico cerca del día de la elección.

El ABM permite capturar **heterogeneidad individual** y **efectos emergentes** que no se observan en modelos agregados.

### 2. Simulación de Monte Carlo
Se utiliza Monte Carlo para:
- Muestrear parámetros inciertos (pesos de influencia, volatilidad, abstención, tamaño de redes).
- Generar múltiples ejecuciones (por ejemplo, 1,000 simulaciones) y obtener distribuciones de resultados.
- Calcular intervalos de confianza y probabilidades de victoria para cada candidato.

Formalmente, si $\theta$ es el vector de parámetros del modelo y $f(\theta)$ es una cantidad de interés (por ejemplo, la cuota de voto final de un candidato), el estimador de Monte Carlo es:

$$
\hat{f}_N = \frac{1}{N} \sum_{k=1}^{N} f(\theta_k), \quad \theta_k \sim p(\theta)
$$

donde $N$ es el número de simulaciones y $p(\theta)$ es la distribución de probabilidad de los parámetros. El error estándar del estimador decrece como:

$$
\text{SE}(\hat{f}_N) = \frac{\sigma_f}{\sqrt{N}}
$$

lo que justifica el uso de un número elevado de ejecuciones para reducir la incertidumbre.

Cada ejecución representa un escenario posible de la elección, lo que permite analizar la robustez de los pronósticos frente a la incertidumbre en las interacciones sociales.

### 3. Niveles de Agregación
El modelo opera en dos niveles complementarios:
- **Nivel individual (agente):** cada votante toma decisiones según sus atributos y su entorno social.
- **Nivel agregado:** los votos se agrupan por sección electoral, distrito federal, entidad federativa y total nacional.

La agregación se define formalmente como:

$$
V_{\text{agregado}} = \sum_{i \in \mathcal{G}} v_i
$$

donde $\mathcal{G}$ es el conjunto de agentes que pertenecen a la unidad territorial considerada y $v_i \in \{0, 1\}$ indica si el agente $i$ votó por el candidato de interés.

Esto facilita la validación con datos reales del INE y permite observar patrones territoriales.

### 4. Interacción Social y Cambio de Decisión
Este es el mecanismo que responde a la pregunta central: **¿cómo y cuándo un votante cambia de opinión?**

#### 4.1. Estructura del entorno social
Cada agente está inmerso en una red social con tres capas:
- **Lazos fuertes:** familiares y amigos cercanos (alta confianza, alta frecuencia de contacto).
- **Lazos débiles:** vecinos, compañeros de trabajo o escuela (confianza media, contacto ocasional).
- **Lazos digitales:** contactos en redes sociales (confianza variable, alta exposición a opiniones diversas).

La red se construye con un modelo de **red libre de escala (Barabási–Albert)** o **small-world (Watts–Strogatz)**, calibrado con datos de redes sociales en México.

#### 4.2. Regla de actualización de opinión: Confianza Acotada (Bounded Confidence)
Un agente $i$ solo es influenciado por otro agente $j$ si la **distancia ideológica** entre ellos es menor que un umbral de tolerancia $\epsilon$:

$$
|x_i - x_j| < \epsilon
$$

donde $x_i$ y $x_j$ son las posiciones ideológicas (escala 0–10). Si la diferencia supera $\epsilon$, el agente $i$ **ignora o rechaza** la opinión de $j$ (efecto de polarización).

#### 4.3. Ecuación de cambio de preferencia
La probabilidad de que el agente $i$ adopte la preferencia del agente $j$ en un paso de tiempo $t$ está dada por:

$$
P(i \to j) = w_{ij} \cdot \frac{1}{1 + |x_i - x_j|} \cdot \mathbb{1}_{|x_i - x_j| < \epsilon}
$$

Donde:
- $w_{ij}$: peso del lazo social (fuerte, débil o digital).
- $\dfrac{1}{1 + |x_i - x_j|}$: penalización por distancia ideológica.
- $\mathbb{1}_{|x_i - x_j| < \epsilon}$: indicador de confianza acotada ($1$ si se cumple, $0$ si no).

#### 4.4. Actualización de la posición ideológica
Cuando el agente $i$ es influenciado por $j$, su posición ideológica se actualiza como un promedio ponderado:

$$
x_i(t+1) = x_i(t) + \mu \cdot w_{ij} \cdot \left( x_j(t) - x_i(t) \right)
$$

donde $\mu \in (0, 1]$ es la **tasa de convergencia** (qué tan rápido se deja influenciar). Si $\mu = 1$, el agente adopta completamente la posición del otro; si $\mu \to 0$, apenas se mueve.

#### 4.5. Factores que modulan la influencia social
| Factor | Efecto en el cambio de decisión |
|---|---|
| **Fuerza del lazo** | Lazos fuertes pesan más que débiles; digitales dependen del algoritmo. |
| **Distancia ideológica** | A mayor distancia, menor probabilidad de cambio (salvo umbral $\epsilon$). |
| **Educación** | Mayor educación → mayor resistencia a la influencia de lazos débiles. |
| **Edad** | Jóvenes más influenciables por lazos digitales; adultos mayores por lazos fuertes. |
| **Vivienda** | Entornos con mejores condiciones → mayor exposición a opiniones diversas. |
| **Ideología** | Agentes extremos tienen umbrales $\epsilon$ más pequeños (menos tolerancia). |

#### 4.6. Dinámica temporal
En cada iteración (un día de campaña):
1. Cada agente selecciona aleatoriamente a un contacto de su red.
2. Se evalúa la regla de confianza acotada: $|x_i - x_j| < \epsilon$.
3. Si se cumple, se actualiza la posición con probabilidad $P(i \to j)$.
4. Se registra el cambio (o no) en la intención de voto.

Este proceso se repite hasta el día de la elección, generando una **trayectoria de decisión** por agente que puede compararse con encuestas panel.

---

##  Modelo Matemático del Voto (Hotelling-Downs)

Como complemento al ABM, el proyecto incorpora el marco de **Hotelling-Downs** reformulado desde la física estadística, que permite calcular la cuota de voto de cada candidato en función de su posición espacial.

### Ecuación base del conteo de votos

La cuota de voto $V$ de un partido se define como la integral de la densidad de votantes $W(\vec{x})$ sobre su región de Voronoi:

$$
V = \iint_{\text{Voronoi}} W(\vec{x}) \, d\vec{x} \tag{1}
$$

### Ecuación con efecto de la abstención

Introduciendo un factor de atenuación por alienación, con parámetro $\tau$ que mide la propensión a la abstención:

$$
V_T = \int_{\text{Voronoi}} \frac{W(\vec{x})}{\left(1 + (\vec{x} - \vec{x_p})^2\right)^{\tau}} \, d\vec{x} \tag{2}
$$

donde $\vec{x_p}$ es la posición del partido. Cuando $\tau \to 0$, la ecuación se reduce a la base (participación total).

### Ecuación con activistas (sesgo hacia los extremos)

Añadiendo un peso adicional $|\vec{x}|^{2\alpha}$ que refleja la influencia desproporcionada de los votantes extremos:

$$
V_{T,A} = \int_{\text{Voronoi}} \frac{W(\vec{x}) \, |\vec{x}|^{2\alpha}}{\left(1 + (\vec{x} - \vec{x_p})^2\right)^{\tau}} \, d\vec{x} \tag{3}
$$

donde $\alpha$ controla la fuerza de la influencia de los extremistas.

### Dinámica de los partidos: Algoritmo de Metropolis

Los partidos ajustan sus posiciones mediante el **algoritmo de Metropolis**, aceptando un nuevo vector de políticas con probabilidad:

$$
P(V_1, V_2) = e^{(V_2 - V_1)/T}
$$

donde $V_1$ es la cuota de voto actual, $V_2$ la nueva, y $T$ es un parámetro de volatilidad análogo a la temperatura.

---

##  Sociodemografía de los Agentes

Los agentes se inicializan con los siguientes atributos, calibrados con datos del INEGI y encuestas nacionales:

| Atributo      | Descripción                                                                 | Valores / Escala                           |
|---------------|-----------------------------------------------------------------------------|--------------------------------------------|
| **Educación** | Nivel educativo máximo alcanzado                                            | Primaria, Secundaria, Bachillerato, Licenciatura, Posgrado |
| **Edad**      | Años cumplidos                                                              | 18-29, 30-44, 45-59, 60+                   |
| **Vivienda**  | Condiciones de la vivienda (acceso a servicios, material, hacinamiento)     | Índice compuesto (0-1)                     |
| **Ideología** | Autoubicación en escala izquierda-derecha                                   | Escala continua 0 (izquierda) a 10 (derecha) |

La probabilidad inicial de voto por un candidato $c$ se modela como una función logística (softmax):

$$
P(v_i = c) = \frac{\exp\left(\beta_c^\top z_i\right)}{\sum_{k \in \mathcal{C}} \exp\left(\beta_k^\top z_i\right)}
$$

donde:
- $z_i$ es el vector de atributos sociodemográficos del agente $i$.
- $\beta_c$ son los coeficientes específicos del candidato $c$ (estimados con encuestas).
- $\mathcal{C}$ es el conjunto de candidatos.

Estos atributos modulan además:
- La **susceptibilidad al cambio** durante la campaña.
- El **tamaño y composición de su red social**.
- El **umbral de tolerancia $\epsilon$** en la interacción social.

---

##  Sistema Electoral: Elecciones Presidenciales en México

El modelo se enfoca en la elección presidencial mexicana, considerando:
- **Candidatos y partidos:** coaliciones y partidos políticos vigentes.
- **Reglas:** mayoría relativa en una sola vuelta (sin segunda vuelta).
- **Territorio:** 300 distritos electorales, 32 entidades federativas.
- **Calendario:** duración de campaña (aprox. 90 días) y eventos clave (debates, cierres de campaña).

La regla de decisión final es:

$$
\text{Ganador} = \arg\max_{c \in \mathcal{C}} \; V_{\text{nacional}}(c)
$$

donde $V_{\text{nacional}}(c)$ es la suma de votos simulados para el candidato $c$ en todas las secciones electorales.

---
