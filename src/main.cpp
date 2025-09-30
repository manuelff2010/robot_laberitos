#include <Arduino.h>
#include <Servo.h>
#define trigger_port 12
#define echo_port 11
#define name "juanito"
#define movement_cost 1
int velocidades[4] = {73,110,73, 110};

Servo S360_1;
Servo S360_2;
Servo S180_1;
Servo S180_2;

struct coord {
  int x; int y;
};

struct node {
    coord data;
    node* next;
};

struct algoritm_node{
    coord data;
    int real_cost;
    int heuristic_cost;
    int total_cost;
    algoritm_node* back_node;
    algoritm_node* next_node;
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

  bool locate_coord(coord wanted){
    node* actual = head;
    while (actual != nullptr) {
        if (actual->data.x == wanted.x && actual->data.y == wanted.y){
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

struct AS_Logic{
    coord map_target;
    algoritm_node* leader_node = nullptr;

    int manhattan_distance(const coord& c1, const coord& c2) const {
        return abs(c1.x - c2.x) + abs(c1.y - c2.y);
    }

    void append_node(algoritm_node* new_node){
        new_node->heuristic_cost = manhattan_distance(new_node->data, map_target);
        new_node->total_cost = new_node->heuristic_cost + new_node->real_cost;
    
        if(leader_node == nullptr){
            leader_node = new_node;
            return;
        }

        if(new_node->total_cost < leader_node->total_cost){
            new_node->next_node = leader_node;
            leader_node = new_node;
            return;
        }

        algoritm_node* current = leader_node;

        while(current->next_node != nullptr && current->next_node->total_cost <= new_node->total_cost){
            current = current->next_node;
        }

        new_node->next_node = current->next_node;
        current->next_node = new_node;
    }

    algoritm_node* min_cost_poper(){
        if(leader_node == nullptr) return nullptr;
        algoritm_node* min_node = leader_node;
        leader_node = leader_node->next_node;
        min_node->next_node= nullptr;

        return min_node;
    }

  void clear() {
    algoritm_node* actual = leader_node;
    algoritm_node* next_data;
    
    while (actual != nullptr){
      next_data = actual->next_node;
      delete actual; 
      actual = next_data;   
    }
    leader_node = nullptr;
    Serial.println("Memoria de la lista liberada.");
  }
};

struct ASA{
  
    node_logic logic_nodes;
    AS_Logic logic;
    
    algoritm_node* find_path(const coord& start, const coord& finish, node*& obstacles){

        logic.map_target = finish;
        algoritm_node* start_node = new algoritm_node;
        
        start_node->data = start;
        start_node->real_cost = 0;       
        start_node->back_node = nullptr;
        start_node->next_node = nullptr;
        
        logic.append_node(start_node);
    
        while(logic.leader_node != nullptr){
            algoritm_node* current = logic.min_cost_poper();

            if(current->data.x == finish.x && current->data.y == finish.y){
                Serial.println("Meta localizada");
                return current;
            }

            int dx[4] = {0, 0, 1, -1}; 
            int dy[4] = {1, -1, 0, 0};

            for(int i = 0; i < 4; ++i){
              coord neighbor_coord = {current->data.x + dx[i], current->data.y + dy[i]};
              if(logic_nodes.locate_coord(neighbor_coord) != true){

                int new_real_cost = current->real_cost + movement_cost;
                // --- (Aquí iría la lógica de 'closed_set' y 'open_set' más compleja) ---
                // Para simplificar, solo crearemos el nodo si tiene un mejor camino
                // (En la implementación completa, se verifica si ya existe en open_set o closed_set)
                
                // Si encontramos un mejor camino o es un nodo nuevo, lo creamos/actualizamos
                
                // Creamos el nuevo nodo temporal (simulando un camino encontrado)
                
                algoritm_node* neighbor_node = new algoritm_node;
                neighbor_node->data = neighbor_coord;
                neighbor_node->real_cost = new_real_cost;
                neighbor_node->back_node = current;
                neighbor_node->next_node = nullptr;
                
                logic.append_node(neighbor_node);
              }
            }
        }

        return nullptr;
    }
    //funcion clear
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

node_logic obstacles;

struct main_logic{
};
void setup(){
}

void loop() {
  //S360_1.write(110);
  //S360_2.write(73); //300
}

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