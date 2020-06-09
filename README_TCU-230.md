# TCU_Secador_Cacao_230
## Universidad de Costa Rica
## Diseño del secador de cacao

El diseño eléctrico consta de 2 partes :

 **A)** Primera parte de control de potencia, este diseño va enfocada hacia la obtención de datos, para su futuro estudio en posibles tesis 
 enfocadas en el proceso de cacao ó cualquier persona que necesite hacer estudio por medio de los datos.
 
 **B)** Segunda parte sistema de alimentación por medio de energía proveniente del sol, para alimentar de manera paralela el controlador  
 electrónico y los extractores de humedad.
 
 **Controlador Electrónico** 
 
El controlador consta de 3 principales etapas, sensado , almacenamiento de datos y toma de decisiones.
**Etapa de Sensado** esta etapa tiene como principal función obtener de una manera muy precisa datos como la temperatura y humedad en ciertos puntos estratégicos del secador de cacao.

Para hacer esto, se utiliza un sensor de humedad y temperatura llamado DHT11, cuyo porcentaje de error en las mediciones de humedad de un  2-5% y en la temperatura de +/- 0.5 grados Celsius.
El primer sensor es colocado a la entrada de aire del secador de cacao, con el fin de obtener los datos del flujo de aire que entra al sistema de secado, pueden ser tomados como referencia inicial.
El Segundo sensor es colocado en el espacio intermedio de las bandejas, cuya función es obtener los datos del proceso que sufren las semillas de cacao durante el todo el proceso de secado
El tercer y último sensor está ubicado en la salida del flujo de aire, contiguo a los extractores, cuya función es en conjunto con los datos del primer sensor se podría hacer estudios del proceso completo de secado.

**Etapa de Almacenamiento** 
Para esta etapa son necesarios los siguientes componentes: keyboard4x4,Ds3231, display LCD 16x02, module microSD y Arduino nano.
La principal función de esta etapa es obtener un archivo CSV (comma separated values) el cual es compatible con Excel para ver importado como tablas ó sino por medio de la librería Pandas y Fitter del lenguaje Python para ser procesado por el mismo y ahí obtener graficas para una así poder analizar con mayor facilidad los datos. 

La función del keyboard para que el productor inserte su código/usuario el cual será guardado en la primera línea del CSV, así la Universidad podrá saber y hacer análisis más certeros del lote de producción, sabiendo la hora y los rangos de temperatura y humedad que se presentaron mediante el proceso .

La función de la LCD 16x02, es una pantalla de cristal líquido que cuenta con 2 filas horizontales cada una con un máximo de 16 caracteres , en ella al inicio mostrará pedir al usuario ingresar su respectivo código , posteriormente mostrara datos como humedad, tiempo de secado y temperatura, es el medio de interacción para el usuario con el controlador.

La función del Ds3231 es un módulo tipo RTC(real time clock), está enfocado en mantener una hora estable para el microcontrolador, que sin importar si se desconecta ó no, este siempre mantendrá su hora y fecha establece. Dicha hora será guardada en la primera columna del CSV 

La función del module microSD es compilar los datos obtenidos mediante los demás sensores en un solo archivos de datos , este este caso el CSV, este módulo escribirá y almacenará sobre la microSD insertada, todos los nuevos datos obtenidos para cada proceso. Será el medio facilitador de datos para la universidad, del cual podrán extraer la microSD y empezar a analizar sus datos internos, para futuros estudios .

**Etapa de toma de decisiones**
 Esta etapa consiste en primeramente cuando el usuario inserta el código correspondiente y le da el botón de iniciar, el microcontrolador , por ahora utilizado el Arduino nano por su simplicidad y bajo precio, procederá a encender los extractores que son controlador por medio de unos relay.
 
 Posteriormente cuando el usuario decida que ya el proceso ha concluido y decida tocar el botón de finalizar , el Arduino nano procederá a apagar los extractores y guardar todos los datos obtenidos en la microSD. El proceso de apagado se dará acabo unos minutos después de haber concluido el proceso de secado, esto es para poder sacar toda la humedad que puede quedar almacenada y así dar una vida útil mayor al secador.
