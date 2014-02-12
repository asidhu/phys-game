#include "sort.h"
#include "body.h"
void sortByPos(body* b){
	//sort by x
	if (b->nextX != 0){
		for (body* tmp = b->nextX; tmp != 0; tmp = tmp->nextX){
			//iterate backwards
			for (body* cursor = tmp->prevX; cursor != NULL; cursor = cursor->prevX){
				if (cursor == b){
					if (tmp->prevX == cursor)
						break;
					tmp->deleteX();
					tmp->insertX(b);
					break;
				}
				if (tmp->AABB.left >= cursor->AABB.left){
					if (tmp->prevX == cursor)
						break;
					//found spot for it, swap and break.
					tmp->deleteX();
					tmp->insertX(cursor);
					break;
				}
			}
		}
	}
}