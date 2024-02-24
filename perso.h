#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

void wait(int t){
    //Fonction qui fait une mini pause, pour fluidifier l'affichage des dialogues
    std::this_thread::sleep_for(std::chrono::milliseconds(t*100));
    //Commenter la ligne au dessus pour ne plus avoir d'attente entre les phrases

}

class item{
    // Classe globale d'item, peut servir pour les potions de vie ou de défense par exemple
public :
    std::string name_;
    std::string type_;
    item(const std::string& name, const std::string& type):name_(name), type_(type){};
    std::string get_type(){return type_;}
    std::string get_name(){return name_;}
    
    void print_info(){
        std::cout << type_ << " : " << name_ << std::endl;
    }

    
};

// TODO : différentes classes d'items, différents type de potion, à intégrer dans le main et le système de combat ensuite



class Armure {
// Ajoute de la défense
public :
    std::string name_;
    int niveau_;
    int protection_;
    Armure(const std::string& name, int niveau, int protection): name_{name}, niveau_(niveau), protection_(protection){};
    
    void print_info(){
        std::cout << name_ << std::endl;
        wait(7);
        std::cout << "Protection : " << protection_ << std::endl;
        wait(7);
        std::cout << "Niveau : " << niveau_ << std::endl;
        wait(7);
    }
    
    int get_defense(){
        return protection_;
    }

    // TODO : système d'augmentation de la défense avec différents niveau
};

class nue : public Armure{

public :
    std::string name_;
    int niveau_;
    int protection_;
    nue():Armure("Nue", 0, 0){};
};

// TODO : autres classes d'armures



//Création des différentes armes
class Arme {

public :
    std::string name_;
    int degats_;
    int portee_;
    int niveau_;
    Arme(const std::string& name, int degats, int portee, int niveau): name_{name}, degats_{degats}, portee_{portee}, niveau_{niveau}{};
    void print_info(){
        std::cout << name_ << std::endl;
        wait(7);
        std::cout<< "Degats : " << degats_ << std::endl;
        wait(7);
        std::cout<< "Portée : " << portee_ << std::endl;
        wait(7);
        std::cout<<std::endl;
    }

    int get_degats(){
        return degats_;
    }

    std::string get_name(){
        return name_;
    }

    // TODO : augmentation de dégats avec les niveaux
    // Système de portée à prendre en compte et mettre dans les comabts

};

class Epee : public Arme{
    public :
    Epee():Arme("Epee", 35, 1, 1){}
};

class Hache : public Arme{
    public :
    Hache():Arme("Hache", 30, 2, 1){}
};

class Arc : public Arme{
    public :
    Arc():Arme("Arc", 20, 5, 1){}
};

class Masse : public Arme{
    public :
    Masse():Arme("Masse", 25, 10, 1){}
};

class Baton : public Arme{
    public:
    Baton():Arme("Baton", 10, 1, 1){}
};
 
class Sceptre : public Arme{
    public :
    Sceptre():Arme("Sceptre", 20, 20, 1){}
};

class Poing : public Arme{
    public :
    Poing():Arme("Poing", 5, 1, 1){}
};

class Dague : public Arme{
    public :
    Dague():Arme("Dague", 25, 1, 1){}
};







//Création des différents personnages
class Personnage 
{
protected:
    std::string name_;
    std::shared_ptr<Arme> arme_{nullptr};
    std::shared_ptr<Armure> armure_{nullptr};
    std::vector<std::shared_ptr<item>> inventaire_{};
    std::string arme_predilection_;
    int vie_;
    int max_vie_;
    int defense_;
    int force_;
    int agilite_;
    int level_;
    int experience_;



public:
    Personnage(const std::string& name,const std::string& arme_predilection, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire, int pv, int max_pv, int defense, int force, int agilite, int level, int experience):
         name_{name}, arme_predilection_{arme_predilection}, arme_{arme}, armure_{armure}, inventaire_{inventaire}, vie_{pv}, max_vie_{max_pv}, defense_{defense}, force_{force}, agilite_{agilite}, level_{level}, experience_{experience}{};
   
    void print_info(){
        std::cout << "Name : " << name_ << std::endl;
        wait(7);
        std::cout << "Arme : " << arme_->name_<< std::endl;
        wait(7);
        std::cout << "Armure : " << armure_->name_ << std::endl;
        wait(7);
        std::cout << "PV : " << vie_ << " / " << max_vie_<< std::endl;
        wait(7);
        std::cout << "Defense : " << defense_ << std::endl;
        wait(7);
        std::cout << "Force : " << force_ << std::endl;
        wait(7);
        std::cout << "Agilite : " << agilite_ << std::endl;
        wait(7);
        std::cout << "Niveau : " << level_ << std::endl;
        wait(7);
        std::cout << "Experience : " << experience_ << std::endl;
        wait(2);
        std::cout << "Inventaire : ";
        wait(7);
        if (inventaire_.size() == 0){
            std::cout << "Vide" << std::endl;
        }
        else{
            for(auto& item : inventaire_){
                std::cout << item->name_ << " ";
            }
        }
        std::cout << std::endl;
        wait(7);
    };
    int get_level(){
        return level_;
    };
    int get_pv(){
        return vie_;
    };
    int get_defense(){
        return defense_;
    };
    int get_force(){
        return force_;
    };
    int get_agilite(){
        return agilite_;
    };
    int get_experience(){
        return experience_;
    };

    void hitted(int damage){
        vie_ -= damage;
    };


    int attack(){
        // TODO : complexifier le système d'attaque
        int degats = force_ + arme_->get_degats();
        if (arme_->get_name() == arme_predilection_){
            return degats * 1.5;
        }
        else{
            return degats;
        }
    };

    int defend(){
        return defense_ + armure_->get_defense();
    }

    void print_inventaire(){
        std::cout << "Inventaire : " << std::endl;
        wait(7);
        // Afficher les items de l'inventaire
        for (auto item : inventaire_){
            std::cout << item->name_ << std::endl;
        }
    };

    std::shared_ptr<Arme> get_arme(){
        return arme_;
    }

    std::shared_ptr<Armure> get_armure(){
        return armure_;
    }


    void add_item(std::shared_ptr<item> new_item){
        inventaire_.push_back(new_item);
        std::cout << "Vous avez ajouté " << new_item->name_ << " à votre inventaire." << std::endl;
        wait(7);
    };

    void select_arme(std::shared_ptr<Arme> nouvelle_arme){
        std::cout<<std::endl;
        std::cout << "Vous aviez :  " << std::endl;
        wait(7);
        arme_->print_info();
        wait(7);
        arme_ = nouvelle_arme;
        std::cout << "Vous changez d'arme, voilà ce que vous avez maintenant :" << std::endl;
        wait(7);
        arme_->print_info();
        std::cout << std::endl;
    };

    void select_armure(std::shared_ptr<Armure> nouvelle_armure){
        std::cout << "Vous aviez : " << std::endl;
        wait(7);
        armure_->print_info();
        wait(7);
        armure_ = nouvelle_armure;
        std::cout << "Vous changez d'armure, voilà ce que vous avez maintenant :" << std::endl;
        wait(7);
        armure_->print_info();
        std::cout<<std::endl;
    };


    void add_experience(int exp){
        experience_ += exp;
    };
    
    void update_lvl(){
        bool test = false;
        if ((experience_>= 100) and (level_ == 1)){
            level_++;
            experience_ -=100;
            test = true;
            
        }
        else if ((experience_>= 200) and (level_ == 2)){
            level_++;
            experience_ -= 200;
            test = true;
        }
        else if ((experience_>= 400) and (level_ == 3)){
            level_++;
            experience_ -= 400;
            test = true;
        }
        else if ((experience_>= 800) and (level_ == 4)){
            level_++;
            experience_ -= 800;
            test = true;
        }
        else if ((experience_>= 1200) and (level_ == 5)){
            level_ ++;
            experience_ -= 1200;
            test = true;
        }
        else if ((experience_>= 1600) and (level_ == 6)){
            level_ ++;
            experience_ -= 1600;
            test = true;
        }
        else if ((experience_>= 2000) and (level_ == 7)){
            level_ ++;
            experience_ -= 2000;
            test = true;
        }
        else if ((experience_>= 2500) and (level_ == 8)){
            level_ ++;
            experience_ -= 2500;
            test = true;
        }
        else if ((experience_>= 3000) and (level_ == 9)){
            level_ ++;
            experience_ -= 3000;
            test = true;
        };
        if (test){
            std::cout << "Vous avez gagné un niveau." << std::endl;
            wait(12);
            std::cout << "Votre êtes passé au niveau " << level_ << "." << std::endl;
            wait(7);
            std::cout << std::endl;
        }
     };
     
     void update(){
        std::cout <<std::endl;
     }
};

// Les différentes classes de personnage

class Chevalier : public Personnage
{
public:
    Chevalier(const std::string& name, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire):
        Personnage{name, "Epee", arme, armure, inventaire, 100, 100, 18, 21, 15, 1, 0}{};
    
    void update(){
        vie_ = vie_ + 15 * level_;
        max_vie_ = max_vie_ + 15 * level_;
        defense_ = defense_ + 4 * level_;
        force_ = force_ + 6 * level_;
        agilite_ = agilite_ + 4 * level_;
        level_ = level_;
    }

    
};

class Mage : public Personnage
{
public:
    Mage(const std::string& name, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire):
        Personnage{name, "Sceptre", arme, armure, inventaire, 75, 75, 13, 12, 25, 1, 0}{};
    
    void update(){
        vie_ = vie_ + 13 * level_;
        max_vie_ = max_vie_ + 13 * level_;
        defense_ = defense_ + 5 * level_;
        force_ = force_ + 4 * level_;
        agilite_ = agilite_ + 4 * level_;
        level_ = level_;
    }
};

class Archer : public Personnage
{
public:
    Archer(const std::string& name, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire):
        Personnage{name, "Arc", arme, armure, inventaire, 75, 75, 10, 15, 35, 1, 0}{};

    void update(){
        vie_ = vie_ + 13 * level_;
        max_vie_ = max_vie_ + 13 * level_;
        defense_ = defense_ + 3 * level_;
        force_ = force_ + 5 * level_;
        agilite_ = agilite_ + 5 * level_;
        level_ = level_;
    }
};

class Assassin : public Personnage
{
public:
    Assassin(const std::string& name, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire):
        Personnage{name, "Dague", arme, armure, inventaire, 50, 50, 15, 18, 50, 1, 0}{};

    void update(){
        vie_ = vie_ + 11 * level_;
        max_vie_ = max_vie_ + 11 * level_;
        defense_ = defense_ + 2 * level_;
        force_ = force_ + 6 * level_;
        agilite_ = agilite_ + 7 * level_;
        level_ = level_;
    }

};

class Barbare : public Personnage 
{
public:
    Barbare(const std::string& name, std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire):
        Personnage{name, "Hache", arme, armure, inventaire, 100, 100, 15, 25, 20, 1, 0}{};

    void update(){
        vie_ = vie_ + 17 * level_;
        max_vie_ = max_vie_ + 17 * level_;
        defense_ = defense_ + 1 * level_;
        force_ = force_ + 5 * level_;
        agilite_ = agilite_ + 2 * level_;
        level_ = level_;
    }
};

//Monstre à affronter
class Monstre : public Personnage
{  
public:
    Monstre(std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire, const int& level):
        Personnage{"Monstre", "Masse", arme, armure, inventaire, 70, 70, 8, 20, 7, level, 0}{};
    void update(){
        vie_ = vie_ + 12 * level_;
        max_vie_ = max_vie_ + 12 * level_;
        defense_ = defense_ + 2 * level_;
        force_ = force_ + 5 * level_;
        agilite_ = agilite_ + 3 * level_;
        level_ = level_;
    }
    void print_ennemi(){
        wait(7);
        std::cout << "Vous faites face à un monste niveau " << level_ << std::endl;
        wait(7);
    }
};



//Générer des Quêtes
class Quete{
    std::string name_;
    std::string description_;
    int element_requis_;
    int statut_;

    public :
    Quete(std::string name, std::string description, int element_requis, int statut){
        name_ = name;
        description_ = description;
        element_requis_ = element_requis;
        statut_ = statut;
    }
    void print_info(){
        std::cout <<  name_ << std::endl;
        wait(2);
        std::cout <<  description_ << std::endl;
        wait(2);
        std::cout << "Statut : " << statut_ <<" / " << element_requis_ << std::endl;
        wait(2);
        std::cout << std::endl;
    }

    void success(){
        // TODO : cas où la quête est finie (à insérer dans update quête)
    }
};



//Créer une pièce
class Piece{
    int numero_;
    int numero_liste_;
    std::string nom_;
    std::vector<int> pieces_voisines_;// l'orientation de la piece étant fixée 0 pour Est, 1 pour Nord, 2 pour Ouest, 3 pour Sud
    std::vector<std::shared_ptr<item>> items_;
    std::vector<std::shared_ptr<Arme>> armes_;
    std::vector<std::shared_ptr<Armure>> armures_;
    std::vector<std::shared_ptr<Monstre>> monstres_;
    int experience_;

    public :
    Piece(int numero, int numero_liste, std::string nom, std::vector<int> pieces_voisines, std::vector<std::shared_ptr<item>> items, std::vector<std::shared_ptr<Arme>> armes, std::vector<std::shared_ptr<Armure>> armures, std::vector<std::shared_ptr<Monstre>> monstres, int experience){
        numero_ = numero;
        numero_liste_ = numero_liste;
        nom_ = nom;
        pieces_voisines_ = pieces_voisines;
        items_ = items;
        armes_ = armes;
        armures_ = armures;
        monstres_ = monstres;
        experience_ = experience;
    }

    std::vector<std::shared_ptr<item>> get_items(){
        return items_;
    }

    std::vector<std::shared_ptr<Arme>> get_armes(){
        return armes_;
    }

    std::vector<std::shared_ptr<Armure>> get_armures(){
        return armures_;
    }

    std::vector<std::shared_ptr<Monstre>> get_monstre(){
        return monstres_;
    }

    int get_experience(){
        return experience_;
    }

    int get_numero(){
        // Numéro de la pièce
        return numero_;
    }

    int get_numero_liste(){
        // Numéro dans l'ordre de découverte des pièces par le joueur
        return numero_liste_;
    }
    void drop_arme(std::shared_ptr<Arme> arme){
        armes_.push_back(arme);
        std::cout << "Vous déposez votre arme dans la salle" << std::endl;
        wait(7);
    }

    void drop_armure(std::shared_ptr<Armure> armure){
        armures_.push_back(armure);
        std::cout << "Vous déposez votre armure dans la salle" << std::endl;
        wait(7);
    }

    bool check_next_piece(int numero){
        if (numero == 0){
            return false;
        }
        // Vérifier si le numéro de pièce est dans la liste des pièces voisines
        for(auto &piece : pieces_voisines_){
            if(piece == numero){
                return true;
            }
        }
        return false;
    }

    
    void print_numero_and_voisins(){
        std::cout << "Vous êtes dans la salle " << numero_ << " et vous pouvez aller aux salles : " << std::endl;
        wait(7);
        for(auto &voisin : pieces_voisines_){
            if (voisin != 0){
                std::cout << voisin << " ";
                wait(7);
            }
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    bool print_item(){
        if (items_.size() == 0){
            std::cout << "Il n'y a pas d'items dans cette pièce" << std::endl;
            wait(7);
            std::cout << std::endl;
            return false;
        }
        std::cout << "Vous trouvez : " << std::endl;
        wait(7);
        int i = 0;
        std::cout<<std::endl;
        for(auto &item : items_){
            wait(7);
            std::cout << "Item numéro " <<i << " : " <<std::endl;
            i++;
            item->print_info();
        }
        std::cout << std::endl;
        return true;
    }


    bool print_armes(){
        if (armes_.size() == 0){
            std::cout << "Il n'y a pas d'armes dans cette pièce" << std::endl;
            wait(7);
            std::cout << std::endl;
            return false;
        }
        std::cout << "Vous voyez un tas d'armes par terre : " << std::endl;
        wait(7);
        int i = 0;
        std::cout<<std::endl;
        for(auto &arme : armes_){
            wait(7);
            std::cout << "Arme numéro " << i << " : " ;
            i++;
            arme->print_info();
        }
        std::cout << std::endl;
        return true;
    }

    bool print_armures(){
        if (armures_.size() == 0){
            std::cout << "Il n'y a pas d'armures dans cette pièce" << std::endl;
            wait(7);
            std::cout << std::endl;
            return false;
        }
        std::cout << "Vous voyez ce qui pourrait être une armure par terre : " << std::endl;
        wait(7);
        int i = 0;
        std::cout << std::endl;
        for(auto &armure : armures_){
            wait(7);
            std::cout << "Armure numéro " << i << " : " ;
            i++;
            armure->print_info();
        }
        std::cout << std::endl;
        return true;
    }

    void print_info(){
        std::cout << "Vous arrivez dans " << nom_ << std::endl;
        wait(15);
        std::cout<<std::endl;
        if (monstres_.size() > 0){
            for(auto &monstre : monstres_){
                wait(7);
                monstre->print_ennemi();
                
            }
            wait(12);
            std::cout << "Vous devez tuer tous les monstres pour pouvoir passer à la pièce suivante" << std::endl;
            std::cout<<std::endl;
            wait(25);
            std::cout << "Le combat va commencer, bonne chance..." << std::endl;
            wait(25);
            std::cout<<std::endl;
        }
    }
};