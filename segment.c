#include "segment.h"
#include <stdlib.h>


MemoryHandler *memory_init(int size){
    MemoryHandler* handler = (MemoryHandler*)malloc(sizeof(MemoryHandler));
    handler->total_size = size;
    handler->free_list = (Segment*)malloc(sizeof(Segment));
    handler->free_list->start = 0;
    handler->free_list->size = size;
    handler->allocated = hashmap_create();
    handler->allocated->size = size;
    handler->memory = (void**)malloc(sizeof(void*)*size)
    return handler;
}


Segment* find_free_segment(MemoryHandler* handler, int start, int size, Segment** prev){
    Segment* prev = NULL;
    Segment* tmp = handler->free_list;
    while(tmp){
        if((tmp->next->start <= start) && (tmp->size >= start+size)){
            return tmp;
        }
        (*prev) = tmp;
        tmp = tmp->next;
    }
    return NULL;
}

int create_segment(MemoryHandler *handler, const char *name, int start, int size){
    Segment* prev = NULL;
    Segment* freeSeg = find_free_segment(handler, start, size, &prev);
    if(freeSeg == NULL){
        return 0;
    }
    Segment* newSeg = (Segment*)malloc(sizeof(Segment));
    newSeg->start = start;
    newSeg->size = size;
    newSeg->next = NULL;

    if((freeSeg->start == start) && (freeSeg->size == size)){
        //supprimer le free seg et relier
        if(prev){
            prev->suiv = freeSeg->suiv;
        }else{
            handler->free_list = freeSeg->suiv;
        }
        free(freeSeg);
        //inserer dans la hashmap
        hashmap_insert(handler->allocated, name, newSeg);
        return 1;
    }
    if((freeSeg->start < start) && (freeSeg->size == size)){
        //reduire la taille du free seg à droite
        freeSeg->size = newSeg->start - freeSeg->start;
        //inserer dans la hashmap
        hashmap_insert(handler->allocated, name, newSeg);
        return 1;
    }
    if((freeSeg->start == start) && (freeSeg->size > size)){
        //reduire la taille du free seg à gauche
        freeSeg->start = freeSeg->start + newSeg->size;
        //inserer dans la hashmap
        hashmap_insert(handler->allocated, name, newSeg);
        return 1;
    }
    if((freeSeg->start == start) && (freeSeg->size > size)){
        //split le free seg (créer deux free seg et supprimer le seg)
            total_free_size = freeSeg->size;
            freeSeg->size = newSeg->start - freeSeg->start

            Segment* rightSeg = (Segment*)malloc(sizeof(Segment));
            rightSeg->start = start + size
            rightSeg->size = total_free_size - size - freeSeg->size;

            //inserer nouveau free seg (relier les pointeurs)
            rightSeg->suiv = freeSeg->suiv;
            freeSeg->suiv = rightSeg;
        //inserer dans la hashmap
        hashmap_insert(handler->allocated, name, newSeg);
        return 1;
    }
    return 0;
}

void fusion_segments(Segment* s1, Segment*s2){
    //on suppose que la fonction est correctement utilisée
    s1->size = s1->size + s2.size;
    s1->suiv = s2->suiv;
    free(s2);
}

int remove_segment(MemoryHandler* handler, const char *name){
    Segment* toDelete = (Segment*)hashmap_get(handler, name);
    delStart = toDelete->start;
    delSize = toDelete->size;
    
    Segment* listeSeg = handler->free_list;
    while(listeSeg){
        if((listeSeg->start + listeSeg->size) == (toDelete->start-1)){
            //on a trouvé un segment libre fusionnable AVANT le segment a supprimer
            //on relie le suivant pour pouvoir fusionner
            toDelete->suiv = listeSeg->suiv;
            fusion_segments(listeSeg, toDelete); /*ici : on fusionne le segment libre avec le segment désalloué en ayant relié avec le segment suivant*/
            return 1;
        }else{
            //simplement ajouter le segment a la liste des segments libres
            //to do
        }
        listeSeg = listeSeg->suiv;
    }
}

