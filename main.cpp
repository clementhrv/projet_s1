#include <iostream>
#include <vector>
#include <memory> //pour pouvoir utiliser shared_ptr
#include <string>
#include <fstream>
#include <random>
#include <chrono>
#include <thread>

#include "perso.h"


auto creer_personnage(const std::string& name, std::string classe,  std::shared_ptr<Arme> arme, std::shared_ptr<Armure> armure, std::vector<std::shared_ptr<item>> inventaire){
    //Création du personnage en fonction de la classe voulue
    std::shared_ptr<Personnage> perso;
    if (classe == "Chevalier"){
        perso = std::make_shared<Chevalier>(name, arme, armure, inventaire);
    }  
    else if (classe == "Mage"){
        perso = std::make_shared<Mage>(name, arme, armure, inventaire);
    }
    else if (classe == "Archer"){
        perso = std::make_shared<Archer>(name, arme, armure, inventaire);
    }
    else if (classe == "Assassin"){
        perso = std::make_shared<Assassin>(name, arme, armure, inventaire);
    }
    else if (classe == "Barbare"){
        perso = std::make_shared<Barbare>(name, arme, armure, inventaire);
    }
    else 
    {
        throw "Erreur : personnage non reconnu";
    }
    std::cout<<std::endl;
    std::cout<<"Très bon choix. Un " << classe <<" devrait facilement s'en sortir. Par ailleurs voici vos statistiques."<< std::endl;
    wait(15);
    perso->print_info();
    return perso;
};



std::vector<int> set_voisins (int piece_actuelle){
    //Pour le moment on suppose le chatteaux comportant 25 pieces (carré de 5x5) numérotées de 1 à 25
    // TODO : rajout d'un paramêtre size pour une taille de chateau définie
    std::vector<int> voisins = {};
    voisins.push_back(piece_actuelle + 5);
    voisins.push_back(piece_actuelle  + 1);
    voisins.push_back(piece_actuelle - 5);
    voisins.push_back(piece_actuelle - 1);
    //Test des bords, on met des 0 pour les murs
    if (piece_actuelle % 5 == 0){
        voisins[3] = 0;
    }
    if (piece_actuelle % 5 == 4){
        voisins[1] = 0;
    }
    if (piece_actuelle <= 5){
        voisins[2] = 0;
    }
    if (piece_actuelle > 20){
        voisins[0] = 0;
    }
    return voisins;
}


std::shared_ptr<Piece> next_piece(int numero_piece, const std::shared_ptr<Personnage>& personnage, int numero_liste){
    // Création d'une nouvelle pièce
    int level = personnage->get_level();
    std::vector<std::shared_ptr<Monstre>> monsters;
    std::vector<std::shared_ptr<item>> items = {};
    std::vector<std::shared_ptr<Arme>> armes = {};
    std::vector<std::shared_ptr<Armure>> armures = {};
    
    // Utilisation de l'aléatoire pour générer les monstres en quantité et d'un niveau adéquate aux caractéristiques du personnage
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution{0, numero_piece};
    int nb_monstre = distribution(gen);
    std::uniform_int_distribution<int> distribution2{level-1, level+2};
    for (int i = 0; i < nb_monstre; i++){
        std::shared_ptr<Armure> pas_darmure2 = std::make_shared<nue>();
        std::vector<std::shared_ptr<item>> pas_ditem2;
        std::shared_ptr<Monstre> monster = std::make_shared<Monstre>(std::make_shared<Poing>(), pas_darmure2, pas_ditem2, distribution2(gen));
        monster->update();
        monsters.push_back(monster);
    }
    // Ajout aléatoire d'armes au sol que le personnage peut récupérer à la fin du combat
    // TODO : Possibilité de mettres plusieurs armes au sol (en fonction du niveau du personnage)
    std::uniform_int_distribution<int> distribution3{0, 5};
    int num_arme = distribution3(gen);
    if (num_arme == 0){
        std::shared_ptr<Arme> arme = std::make_shared<Epee>();
        armes.push_back(arme);
    }
    else if (num_arme == 1){
        std::shared_ptr<Arme> arme = std::make_shared<Hache>();
        armes.push_back(arme);
    }
    else if (num_arme == 2){
        std::shared_ptr<Arme> arme = std::make_shared<Masse>();
        armes.push_back(arme);
    }
    else if (num_arme == 3){
        std::shared_ptr<Arme> arme = std::make_shared<Arc>();
        armes.push_back(arme);
    }
    else if (num_arme == 4){
        std::shared_ptr<Arme> arme = std::make_shared<Baton>();
        armes.push_back(arme);
    }
    else if (num_arme == 5){
        std::shared_ptr<Arme> arme = std::make_shared<Sceptre>();
        armes.push_back(arme);
    }

    std::shared_ptr<Piece> piece_suivante = std::make_shared<Piece>(numero_piece, numero_liste, "Piece", set_voisins(numero_piece),  items, armes, armures,  monsters, nb_monstre*100);
    return piece_suivante;
}


bool dodge(std::shared_ptr<Personnage> perso){
    // Possibilité pour le personnage d'esquiver une attaque en fonction de son agilité, sinon il prendra tous les dégats
    int agilite = perso->get_agilite();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution{0, 100};
    if (agilite > distribution(gen)){
        return false;
    }
    else{
        return true;
    }
}



void fight_victory(std::shared_ptr<Personnage> &perso, std::shared_ptr<Piece> &piece_atcuelle){
    std::vector<std::shared_ptr<item>> items = piece_atcuelle->get_items();
    std::vector<std::shared_ptr<Arme>> armes = piece_atcuelle->get_armes();
    std::vector<std::shared_ptr<Armure>> armures = piece_atcuelle->get_armures();
    // Suite à la victoire (ou retour dans la pièce quand la victoire a déjà été acquise avant), on peut récupérer les items, les armes et les armures au sol (possibilité de faire un choix) 

    // Pour les items
    bool test = piece_atcuelle->print_item();
    wait(7);
    while (test){
        std::cout <<"Voulez vous prendre un objet ? (oui/non)"<<std::endl;
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "oui"){
            std::cout << "Quel objet voulez vous prendre ? (donner le numéro)" << std::endl;
            std::string answer2;
            std::getline(std::cin, answer2);
            int numero = std::stoi(answer2);
            if (numero >= 0 && numero < items.size()){
                perso->add_item(items[numero]);
                items.erase(items.begin()+numero);
                test = false;
            }
            else{
                std::cout << "Ce numéro n'existe pas" << std::endl;
            } 
        }
        else if (answer == "non"){
            test = false;
        }
        else{
            std::cout << "Veuillez répondre par oui ou non" << std::endl;
        }
    };
    std::cout<<std::endl;

    // Pour les armes
    test = piece_atcuelle->print_armes();
    wait(7);
    while (test){
        std::cout <<"Voulez vous prendre une arme ? (oui/non)"<<std::endl;
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "oui"){
            std::cout << "Quel arme voulez vous prendre ? (donner le numéro)" << std::endl;
            std::string answer3;
            std::getline(std::cin, answer3);
            int numero = std::stoi(answer3);
            if (numero >= 0 && numero < armes.size()){
                std::shared_ptr dropped = perso->get_arme();
                piece_atcuelle->drop_arme(dropped);
                armes.push_back(dropped);
                perso->select_arme(armes[numero]);
                armes.erase(armes.begin()+numero);
                test = false;
            }
            else{
                std::cout << "Ce numéro n'existe pas" << std::endl;
            }
        }
        else if (answer == "non"){
            test = false;
        }
        else{
            std::cout << "Veuillez répondre par oui ou non" << std::endl;
        }
    };
    std::cout<<std::endl;

    // Pour les armures
    test = piece_atcuelle->print_armures();
    wait(7);
    while (test){
        std::cout <<"Voulez vous prendre une armure ? (oui/non)"<<std::endl;
        std::string answer;
        std::getline(std::cin, answer);
        if (answer == "oui"){
            std::cout << "Quel armure voulez vous prendre ? (donner le numéro)" << std::endl;
            std::string answer4;
            std::getline(std::cin, answer4);
            int numero = std::stoi(answer4);
            if (numero >= 0 && numero < armures.size()){
                std::shared_ptr dropped = perso->get_armure();
                piece_atcuelle->drop_armure(dropped);
                armures.push_back(dropped);
                perso->select_armure(armures[numero]);
                armures.erase(armures.begin()+numero);
                test = false;
            }
            else{
                std::cout << "Ce numéro n'existe pas" << std::endl;
            }
        }
        else if (answer == "non"){
            test = false;
        }
        else{
            std::cout << "Veuillez répondre par oui ou non" << std::endl;
        }
    };
    std::cout << std::endl;
    std::cout << "Voici ce que vous avez maintenant :" << std::endl;
    wait(7);
    perso->print_info();
    wait(7);
    std::cout << std::endl;
    std::cout << "Vous pouvez désormais allez explorer le reste du chateau." << std::endl;
}


int startFight(std::shared_ptr<Personnage> &perso, std::shared_ptr<Piece> &piece_atcuelle){
    std::vector<std::shared_ptr<Monstre>> monsters = piece_atcuelle->get_monstre();
    if(monsters.size()==0){
        std::cout<<"La salle est vide"<<std::endl;
        return 0;
    }
    
    bool fight_continue = true;
    int status = 0; //alive = 0 /dead = 1

    // Affichage pour le début du combat
    std::string temp;
    wait(5);
    std::cout<<" ##############################################" <<std::endl;
    std::cout<<" #                                            #" <<std::endl;
    std::cout<<" # Appuyez sur entrée pour démarrer le combat #" <<std::endl;
    std::cout<<" #                                            #" <<std::endl;
    std::cout<<" ##############################################" <<std::endl;
    std::getline(std::cin, temp);

    while (fight_continue){        
        std::cout<<"Vous avez "<<perso->get_pv()<<" PV"<<std::endl;
        wait(7);
        // Cas où les monstres sont nombreux
        // TODO : Possibilité d'utiliser une potion ou autre à la place d'attaquer (comme dans Pokémon)
        // TODO : Choix de l'attaque et coup critique
        if (monsters.size() > 1){
            std::cout<<"Il y a "<<monsters.size()<<" monstres, qui ont "<<std::endl;
            wait(7);
            for (int i = 0; i < monsters.size(); i++){
                std::cout<<monsters[i]->get_pv()<<" PV"<<std::endl;
                wait(7);
            }
            std::cout<<std::endl;
            std::cout<<"Vous attaquez"<<std::endl;
            wait(12);
            std::cout<<"Choisissez un monstre à attaquer (donnez un numéro entre 0 et "<< monsters.size() - 1<< ")"<<std::endl;
            wait(7);
            std::string num;
            std::getline(std::cin, num);
            std::cout<<std::endl;
            int numero = std::stoi(num); // Attention !! si le champ saisi n'est pas un nombre provoque une erreur
            // Cas où le numéro n'est pas valide
            if (numero >= monsters.size() or numero < 0){
                std::cout<<"Vous avez choisi un monstre inexistant"<<std::endl;
                wait(7);
                std::cout<<"Choisissez un monstre à attaquer (donnez un numéro entre 0 et "<< monsters.size() - 1<< ")"<<std::endl;
                wait(7);
                std::string num;
                std::getline(std::cin, num);
                std::cout<<std::endl;
                numero = std::stoi(num);
                if (numero>= monsters.size() or numero < 0){
                    std::cout<<"Erreur"<<std::endl;
                    status = 1;
                    return status;
                }
            }
            
            monsters[numero]->hitted(perso->attack()-monsters[numero]->defend());
            std::cout<<"Vous infligez " << perso->attack()-monsters[numero]->defend() << " points de dégats"<<std::endl;
            wait(7);
            if (monsters[numero]->get_pv() <= 0){
                monsters.erase(monsters.begin() + numero);
            }
        }
        // Idem pour un seul monstre
        else {
            std::cout<<"Il y a un monstre qui a "<< monsters[0]->get_pv()<<" PV"<<std::endl;
            std::cout<<std::endl;
            wait(7);
            std::cout<<"Vous attaquez"<<std::endl;
            wait(12);
            monsters[0]->hitted(perso->attack()-monsters[0]->defend());
            std::cout<<"Vous infligez " << perso->attack()-monsters[0]->defend() << " points de dégats"<<std::endl;
            wait(7);
            if (monsters[0]->get_pv() <= 0){
                monsters.erase(monsters.begin());
            }
        }
        //  Defense, choix entre parez avec l'armure ou essayer d'esquiver
        for (int i = 0; i < monsters.size(); i++){
            std::cout<<"Le monstre "<<i<<" vous attaque. Vous pouvez parez ou tenter d'esquiver (1 ou 2)"<<std::endl;
            std::string num;
            std::getline(std::cin, num);
            std::cout<<std::endl;
            int action = std::stoi(num);
            if (action == 1){
                perso->hitted(monsters[i]->attack()-perso->defend());
                std::cout<<"Vous vous défendez et perdez "<<monsters[i]->attack()-perso->defend()<<" PV"<<std::endl;  
                wait(7);
            }
            else if (action == 2){
                if (not(dodge(perso))){
                    perso->hitted(monsters[i]->attack());
                    std::cout<<"Vous tantez d'equivez mais c'est un échec. Vous perdez "<<monsters[i]->attack()<<" PV"<<std::endl;
                    wait(7);
                }
                else {
                    std::cout<<"Vous esquivez le coup avec succes"<<std::endl;
                    wait(7);
                }
            }
            else{
                std::cout<<"Action non reconnue"<<std::endl;
            }
        }
        if (perso->get_pv() <= 0){
            std::cout<<"Vous êtes mort"<<std::endl;
            status = 1;
            fight_continue = false;
        }
        else if (monsters.size() == 0){
            status = 0;
            // Victoire, évolution du personnage avec l'expérience
            fight_continue = false;
            int experience = piece_atcuelle->get_experience();
            std::cout<<"Vous avez gagné le combat"<<std::endl;
            std::cout<<std::endl;
            wait(7);
            std::cout<<"Vous gagnez "<< experience << " points d'expérience." <<std::endl;
            wait(7);
            perso->add_experience(experience);
            perso->update_lvl();
            perso->update();
            std::cout<<std::endl;
        }
    }
    return status;
}


void update_quete(){
    //TODO
}

void print_quete(){
    //TODO
}




int startGame(const int &lap) 
{
    //Initialisation du personnage, Introduction
    std::cout <<"Bienvenue dans le chateau hanté."<<std::endl;
    wait(10);
    std::cout <<"Vous êtes un jeune héros qui doit affronter des monstres pour pouvoir sortir vivant du chateau."<<std::endl;
    wait(15);
    std::cout<<"Mais avant toute chose, quel est votre nom ? "<<std::endl;
    std::string name;
    std::getline(std::cin, name);
    std::cout<<std::endl;
    wait(7);
    std::cout<<"Et quel type de héros êtes vous ? (parmis Chevalier, Mage, Archer, Assassin, Barbare): ";
    std::string classe;
    std::getline(std::cin, classe);
    wait(7);

    //Attribution d'une arme de base : le poing => une vraie arme sera ensuite à trouver dans le jeu
    std::shared_ptr<Arme> arme_de_base;
    arme_de_base = std::make_shared<Poing>();
    std::shared_ptr<Armure> pas_darmure = std::make_shared<nue>();
    std::vector<std::shared_ptr<item>> pas_ditem;
    std::shared_ptr<item> premier_item  = std::make_shared<item>("Petite potion", "Potion");
    pas_ditem.push_back(premier_item);
    std::shared_ptr<Personnage> perso = creer_personnage(name, classe, arme_de_base, pas_darmure, pas_ditem);


    //Initialisation des premières quêtes
    wait(7);
    std::cout << std::endl;
    std::cout<<"################################################################################################################"<<std::endl;
    std::cout<<std::endl;
    wait(2);
    std::cout << "Vous avez différentes quêtes que vous devez compléter" << std::endl;
    std::cout << std::endl;
    wait(12);
    std::shared_ptr<Quete> quete_principale = std::make_shared<Quete>("Trouver la sortie", "Vous devez trouver la sortie du chateau", 1, 0);
    quete_principale->print_info();
    wait(10);
    std::shared_ptr<Quete> quete_secondaire_1 = std::make_shared<Quete>("Trouver l'arme", "Vous devez trouver l'arme de votre personnage", 1, 0);
    quete_secondaire_1->print_info();
    wait(10);
    std::shared_ptr<Quete> quete_secondaire_2 = std::make_shared<Quete>("Tuer 5 monstres", "Vous devez tuer 5 monstres", 5, 0);
    quete_secondaire_2->print_info();
    wait(10);
    std::shared_ptr<Quete> quete_secondaire_3 = std::make_shared<Quete>("Visite du chateau", "Vous devez visiter 10 pieces du chateau", 10, 0);
    quete_secondaire_3->print_info();
    wait(5);
    std::cout<<"################################################################################################################"<<std::endl;
    std::cout<<std::endl;
    wait(10);
    
    //Initialisation de la première pièce
    // Stuff dans la pièce
    std::shared_ptr<Armure> pas_darmure2 = std::make_shared<nue>();
    std::vector<std::shared_ptr<item>> pas_ditem2;
    std::shared_ptr<Monstre> monstre_de_base = std::make_shared<Monstre>(std::make_shared<Poing>(), pas_darmure2, pas_ditem2, 1);
    monstre_de_base->update();
    // Monstre dans la pièce
    std::vector<std::shared_ptr<Monstre>> monstre_base = {};
    monstre_base.push_back(monstre_de_base);
    std::vector<std::shared_ptr<item>> item_piece_1 = {};
    std::vector<std::shared_ptr<Arme>> arme_piece_1 = {};
    std::vector<std::shared_ptr<Armure>> armure_piece_1 = {};
    std::shared_ptr<Arme> arme_0 = std::make_shared<Baton>();
    arme_piece_1.push_back(arme_0);
    std::shared_ptr<Piece> piece_1 = std::make_shared<Piece>(1, 0, "l'entrée",  set_voisins(1), item_piece_1, arme_piece_1, armure_piece_1, monstre_base, 100);
    wait(7);
    piece_1->print_info();

    int size = 5; // la taille du chateau, ici 5x5

    std::shared_ptr<Piece>  current_piece = piece_1;
    std::vector<bool> piece_visitee = {false};
    for (int i = 0; i < size*size; i++){
        piece_visitee.push_back(false);
    } //Pour savoir les pieces déjà créée et visitée

    std::vector<std::shared_ptr<Piece>> piece_list = {piece_1}; //Pour stocker toutes les pièces déjà visitées

    bool continuate = true;
    // Tant que le joueur est en vie
    while (continuate){
        int statut = 0;
        std::cout << std::endl;
        // On ne recréer pas de nouvelles pieces à chaque fois, on retourne dans celles déjà visitées
        if(piece_visitee[current_piece->get_numero() - 1]){
            std::cout << "Vous avez déjà visité cette pièce" << std::endl;
            wait(7);
            std::cout<<std::endl;
        }
        else{
            // Sinon si c'est une nouvelle pièce on fait le combat
            statut = startFight(perso, current_piece);
            piece_visitee[current_piece->get_numero() - 1] = true;
        }
        int numero_current_piece = current_piece->get_numero_liste();
        piece_list[numero_current_piece] = current_piece;
        if(statut == 1){
            // Mort
            std::cout << "Fin du jeu" << std::endl;
            return 0;
        }
        else{
            // Récupérer le stuff
            fight_victory(perso, current_piece);
            current_piece->print_numero_and_voisins();
            wait(7);
            // Choix de la prochaine pièce
            std::cout << "Dans quelle piece désirez vous aller ? " << std::endl;
            std::string numero_next_piece_txt;
            std::getline(std::cin, numero_next_piece_txt);
            int numero_next_piece = std::stoi(numero_next_piece_txt);

            //Test pour voir si la piece demandée est accessible réellement
            bool check = current_piece->check_next_piece(numero_next_piece);
            while (not(check)){
                std::cout << "Ceci n'est pas une piece" << std::endl;
                wait(12);
                std::cout << "Dans quelle piece désirez vous aller ? " << std::endl;
                std::getline(std::cin, numero_next_piece_txt);
                numero_next_piece = std::stoi(numero_next_piece_txt);
                check = piece_1->check_next_piece(numero_next_piece);
            }
            if (piece_visitee[numero_next_piece-1] == false){
                std::shared_ptr<Piece> next_place = next_piece(numero_next_piece, perso, piece_list.size());
                piece_list.push_back(next_place);
                current_piece = next_place;
            }
            else{
                for(int i = 0; i < piece_list.size(); i++){
                    if(piece_list[i]->get_numero() == numero_next_piece){
                        current_piece = piece_list[i];
                    }
                }
            }
            
        }
    }
   
    return 0;
}

int main(){

    const int lap = 1000;

    startGame(lap);
    return 0; //end of the game
}