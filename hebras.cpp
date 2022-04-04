#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <chrono>

using namespace std;

//clase para el leer los archivos secuencialmente.
class LectorArchivoSecuencial
{
  private:
    int lineasTotal;
    int palabrasTotal;
    int caracteresTotal;
    char* archivo;
  public:

  LectorArchivoSecuencial()
  {
    lineasTotal = 0;
    palabrasTotal = 0;
    caracteresTotal = 0;
  }

  //metodo para contar las lineas, palabras y caracteres del archivo
  void leer()
  {
    char c[2];
    string line;
    int lineCount = 0;
    string word;
    int wordCount = 0;
    string letter;
    int charCount = 0;


    int stream  = open(archivo, O_RDONLY);

    if(stream == -1)
    {
      std::cout << "Error al abrir archivo" << std::endl;
      return;
    }

    while(read(stream,c,1))
    {
      if(c[0] == '\n')
      {
        lineCount++;
        lineasTotal++;
        wordCount++;
        palabrasTotal++;
      }
      else if(c[0] == ' ')
      {
        wordCount++;
        palabrasTotal++;
      }
      else if((c[0] >= 'a' && c[0] <= 'z')||(c[0]>='A' && c[0]<='Z'))
      {
        charCount++;
        caracteresTotal++;
      }
    }
    close (stream);

    std::ostringstream oss;
    oss << lineCount;
    line = oss.str();
    std::ostringstream oss1;
    oss1 << wordCount;
    word = oss1.str();
    std::ostringstream oss2;
    oss2 << charCount;
    letter = oss2.str();


    string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

    std::cout << "texto: " << archivo << ", " << str;

    }

    //metodo para setear el archivo, asi cambiamos de por cada archivo y vamos sumando sus totales
    void setArchivo(char* param)
    {
      archivo = param;  
    }

    //metodo para printear el total.
    void printTotal()
    {
      string letter;
      string word;
      string line;

      std::ostringstream oss;
      oss << lineasTotal;
      line = oss.str();
      std::ostringstream oss1;
      oss1 << palabrasTotal;
      word = oss1.str();
      std::ostringstream oss2;
      oss2 << caracteresTotal;
      letter = oss2.str();

      string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

      std::cout << "total: " << str;

    }
};

//struct para guardar los datos de los archivos.
typedef struct 
{
  char *archivo;
  int lineas;
  int palabras;
  int caracteres;
}DatosArchivo;

//metodo para contar las lineas, palabras y caracteres por medio de hebras
void *lectorArchivoThread (void *param) {
  DatosArchivo *archivo;
  char c[2];
  string line;
  int lineCount = 0;
  string word;
  int wordCount = 0;
  string letter;
  int charCount = 0;

  archivo = (DatosArchivo *) param;

  int stream = open (archivo->archivo, O_RDONLY);

  if (stream == -1) {
    std::cout << "Error al abrir archivo" << std::endl;
    pthread_exit(0);
  }

  while (read(stream,c,1)) {
    if(c[0] == '\n')
    {
      lineCount++;
      wordCount++;
    }
    else if(c[0] == ' ')
    {
      wordCount++;
    }
    else if((c[0] >= 'a' && c[0] <= 'z')||(c[0]>='A' && c[0]<='Z'))
    {
      charCount++;
    }
  }
  close (stream);

  archivo->lineas = lineCount;
  archivo->palabras = wordCount;
  archivo->caracteres = charCount;

  pthread_exit(0);
}

//metodo para printear el total conseguidos por las hebras
void printTotal(int lTotal, int pTotal, int cTotal)
{
  string letter;
  string word;
  string line;

  std::ostringstream oss;
  oss << lTotal;
  line = oss.str();
  std::ostringstream oss1;
  oss1 << pTotal;
  word = oss1.str();
  std::ostringstream oss2;
  oss2 << cTotal;
  letter = oss2.str();

  string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

  std::cout << "total: " << str;
}


int main(int argc, char *argv[]) {

  //empezamos por la secuencial
  std::cout << "Secuencial" << '\n';
  auto start = chrono::system_clock::now();

  LectorArchivoSecuencial lector1 = LectorArchivoSecuencial();

  for(int i = 0; i < argc -1; i++)
  {
    lector1.setArchivo(argv[i+1]);
    lector1.leer();
  }

  lector1.printTotal();

  auto end = chrono::system_clock::now();
  chrono::duration<double> segundos = end - start;

  std::cout << "segundos: " << segundos.count() << '\n';

  //ahora pasamos a las hebras
  std::cout << "Por hebras." << '\n';
  start = chrono::system_clock::now();

  int lineasTotal = 0;
  int palabrasTotal = 0;
  int caracteresTotal = 0;
  
  DatosArchivo datos[argc-1];

  for(int i = 0; i<argc -1; i++)
  {
    datos[i].archivo = argv[i+1];
    datos[i].lineas = 0;
    datos[i].palabras = 0;
    datos[i].caracteres = 0;
  }
  pthread_t threads[argc - 1];
  int i = 0;
  
  //Crea todos los hilos 
  for (i=0; i < argc - 1; i++) {
    pthread_create(&threads[i], NULL, lectorArchivoThread, &datos[i]);
  }


  //Para esperar por el término de todos los hilos 
  for (i=0; i< argc - 1; i++) {
    pthread_join(threads[i], NULL);
  }

  //para conseguir el numero de lineas, palabras y letras de las estructuras de datos,
  //printearlas y despues sumarselas al total
  for(i = 0; i<argc-1;i++)
  {
    string line;
    string word;
    string letter;

    std::ostringstream oss;
    oss << datos[i].lineas;
    line = oss.str();
    std::ostringstream oss1;
    oss1 << datos[i].palabras;
    word= oss1.str();
    std::ostringstream oss2;
    oss2 << datos[i].caracteres;
    letter = oss2.str();


    string str = "lineas: " + line + ", palabras: " + word + ", caracteres: " + letter  + "\n";

    std::cout << "texto: " << datos[i].archivo << ", " << str;

    lineasTotal = lineasTotal + datos[i].lineas;
    palabrasTotal = palabrasTotal + datos[i].palabras;
    caracteresTotal = caracteresTotal + datos[i].caracteres;
  }

  printTotal(lineasTotal,palabrasTotal,caracteresTotal);
  end = chrono::system_clock::now();
  
  segundos = end - start;
  std::cout << "segundos: " << segundos.count() << '\n';

  return 0;
}