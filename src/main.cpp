#include <Arduino.h>
#include <Servo.h>
#define trigger_port 12
#define echo_port 11
#define name "juanito"

int velocidades[4] = {73,110,73, 110};

Servo S360_1;
Servo S360_2;
Servo S180_1;
Servo S180_2;

struct coord {
  int x;
  int y;
};

struct node {
    coord data;
    node* next;
};

struct AStarNode {
    coord data;     // Coordenadas (x, y)
    int g_score;    // Coste real desde el inicio
    int h_score;    // Coste heurístico (Distancia de Manhattan a la meta)
    int f_score;    // Coste total: g_score + h_score
    AStarNode* parent; // Puntero al nodo anterior (para reconstruir el camino)
    AStarNode* next;   // Puntero al siguiente nodo en la Lista Enlazada
};

struct hc04_controller {
  int trigger;
  int echo;

  void init() {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
  }

  float Get_distance() {  
    digitalWrite(trigger, 0);
    delayMicroseconds(2);
    digitalWrite(trigger, 1);
    delayMicroseconds(10);
    digitalWrite(trigger, 0);
  
    unsigned long distancia = pulseIn(echo, 1) / 58;
  
    return (float)distancia;
  }
};

struct node_logic{
  node* head = nullptr; 

  void append_coord(const coord& new_cord){
    
    node* new_node = new node; 
    
    if (new_node == nullptr) {
        Serial.println("¡ERROR! No hay suficiente RAM para un nuevo nodo.");
        return;
    }

    new_node->data.x = new_cord.x;
    new_node->data.y = new_cord.y;
    new_node->next = nullptr;

    if (head == nullptr) {
        head = new_node;
        Serial.println("Coordenada agregada. Lista inicializada.");
        return;
    }

    node* actual = head;

    while (actual->next != nullptr) {
        actual = actual->next;
    }

    actual->next = new_node;
  }

  bool locate_coord(coord wanted) const{
      node* actual = head;
      
      while (actual != nullptr) {
          if (actual->data.x == wanted.x && actual->data.y == wanted.y) {
              return true;
          }
      
          actual = actual->next;
      }

      return false;
  }

  void clear() {
    node* actual = head;
    node* next_data;
    
    while (actual != nullptr) {
        next_data = actual->next;
        delete actual;            
        actual = next_data;       
    }
    head = nullptr;
    Serial.println("Memoria de la lista liberada.");
  }

  void print_list() const {
    Serial.print("LISTA: ");
    node* actual = head;
    while (actual != nullptr) {
        Serial.print("(");
        Serial.print(actual->data.x);
        Serial.print(", ");
        Serial.print(actual->data.y);
        Serial.print(")");
        if (actual->next != nullptr) {
            Serial.print(" -> ");
        }
        actual = actual->next;
    }
    Serial.println();
  }
};

struct a_star_logic{
  coord map_size;
  node_logic obstacles;
  coord inicio;
  coord finish;

  int manhattan_distance(const coord& c1, const coord& c2) const {
    return abs(c1.x - c2.x) + abs(c1.y - c2.y);
  }

  void border_build(){
  }
};

node_logic my_coords;

void setup() {
    Serial.begin(9600);
    while (!Serial) { ; }
    Serial.println("--- PRUEBA DE LISTA ENLAZADA ---");

    
    coord c1 = {1, 1};
    my_coords.append_coord(c1);

    my_coords.append_coord({50, 75});
    my_coords.append_coord({100, 20});

    my_coords.print_list();

    Serial.println("--- PRUEBA DE BÚSQUEDA ---");
    
    coord target_found = {50, 75};
    if (my_coords.locate_coord(target_found)) {
        Serial.print("Buscando (50, 75) -> ENCONTRADA.");
    } else {
        Serial.println("Buscando (50, 75) -> NO ENCONTRADA.");
    }

    coord target_not_found = {0, 0};
    if (my_coords.locate_coord(target_not_found)) {
        // ...
    } else {
        Serial.print("Buscando (0, 0) -> NO ENCONTRADA.");
    }

    my_coords.clear();
}

void loop() {
  //S360_1.write(110);
  //S360_2.write(73); //300
  //delay(300);
  //S360_1.write(73);
  //S360_2.write(110);
  //delay(300);
}



struct logical_system{
  int* generaL_array;
  int* vecinos_array;
  int* obstacles_array;
  int map_max_x;
  int map_max_y;
  coord initial_position;
  coord finish_position;
  int cost(int a_position[2], int b_position[2]){
    return (a_position[0] - b_position[0]) + (a_position[1] - b_position[1]);
  }

  int* adjacents(){

  }

};

struct servo_360_controller{
  int back_velocity;
  int front_velocity;
};

struct general_controller{
  int servos_pins[4];
  void init(){

    Serial.begin(9600);

    Serial.print("Hello, I'm ");
    Serial.println(name);
    
    hc04_controller hc04;
    hc04.trigger = trigger_port;
    hc04.echo = echo_port;
    hc04.init();
    
    Serial.print(name);
    Serial.println(" can recognize distance");

    S360_1.attach(servos_pins[0]);
    S360_2.attach(servos_pins[1]);
    S180_1.attach(servos_pins[2]);
    S180_2.attach(servos_pins[3]);

    Serial.print(name);
    Serial.println(" feels his muscles");   
 
    S180_1.write(170);
    S180_2.write(0);
    
    Serial.print(name);
    Serial.println(" stand up");
  }
};