Actividad:

El objetivo es implementar máquinas de estado tanto en el software como en el firmware para el manejo de menus y teclados
• Rama develop:
1- Configurar la IA de Github Copilot
2- Verificar archivo .gitignore para que no se suban archivos tipo compilacion
3- Crear archivo YML para compilacion automática en Github cuando se haga push
4- Agregar en Firmware los archivos .c y .h para crear las funciones relacionadas al menú de usuario y los archivos .c y .h para el manejo del teclado.
5- Hacer commit, push, y luego cada desarrollador actualizará su rama y agregará las siguientes funciones:
•   Dev-1: deberá crear la máquina de estados para el menú del software. Realizar el diagrama de estados, teniendo en cuenta las diferentes funciones que tendrá a futuro el proyecto, ejemplo: leer estado sensores, activar actuadores, cargar nuevo usuario, consultar registro de usuarios, etc.
•   Dev-2: Realizar el diagrama de estados para el manejo de un teclado matricial de 4x4. Implementar en el Firmware dicha máquina de estados.
•   Dev-3: Crear la máquina de estados para el menú del firmware. Realizar el diagrama de estados, teniendo en cuenta las diferentes funciones que tendrá a futuro el proyecto.
6- Cada desarrollador debe subir el diagrama de máquinas de estado (como imagen) en documentación.
7- Una vez finalizada su parte, cada desarrollador deberá realizar commit (poner como mensaje E05-FiniteMachineState Dev1 2 o 3 según corresponda) e integrar sus cambios en la rama develop.
8- Hacer los ajustes necesarios para que quede correctamente funcionando el firmware, de tal forma que lo ingresado por teclado permita manejar el menú de usuario.
9- Publicar cambios (push)