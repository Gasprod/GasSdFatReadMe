#include <SPI.h>
#include <SdFat.h>

SdFat sd;


#define SDCS 10
#define BUFFER_SIZE 100
#define LED_STATUS 9
uint8_t buf[BUFFER_SIZE];

void setup() {
  SdFile fichier;
  Serial.begin(115200);

  Serial.println("Initialisation SD...");
  if (!sd.begin(SDCS,SPI_HALF_SPEED)) {
    Serial.println("Erreur initialisation");
    return;
  }else
    pinMode(LED_STATUS, OUTPUT);
  }
  
  /*  Volume total
   *  ************
   */
  // taille d'un cluster en nombre de blocs ->
  uint32_t volume = int(sd.vol()->blocksPerCluster());  
  // ...fois le nombre de clusters ->
  volume *= sd.vol()->clusterCount();          
  // ...fois la taille d'un blocs (toujours 512 octets) ->
  volume *= 512;                               
  
  Serial.print("Taille du volume: ");
  // en mégaoctets ->
  Serial.print(volume/1024/1024);              
  Serial.println(" Mo");

  /*  Espace libre
   *  ************
   */
   
  uint32_t volumelibre = sd.vol()->freeClusterCount();
  volumelibre *= int(sd.vol()->blocksPerCluster());
  volumelibre *= 512;
  
  Serial.print("Espace libre: ");
  Serial.print(volumelibre/1024/1024);
  Serial.println(" Mo");

  // Listage de la racine
  Serial.println(" ");
  sd.ls("/", LS_SIZE|LS_R);
  Serial.println(" ");

  // Ouverture du fichier
  if (!fichier.open(&sd, "HackSdFat.txt", O_READ)) {
    Serial.println("Erreur ouverture fichier");
    return;
  }

  /*
  // Lecture du fichier version read()
  int n = fichier.read(buf, sizeof(buf));
  Serial.print(n);
  Serial.println(" octet(s) lu(s)");
  */

  // Lecture du fichier version fgets()
  int n = fichier.fgets(buf, sizeof(buf));
  Serial.print(n);
  Serial.println(" octet(s) lu(s)");

  String myString = String((char *)buf);
  myString.trim();
  
  Serial.print("\"");
  Serial.print(myString);
  Serial.println("\"");

  /// Fermeture du fichier
  fichier.close(); 
}

void loop() {
}

