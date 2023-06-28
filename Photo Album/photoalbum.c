/* Tanav Marupudi tanav 118051659 */

#include <stdio.h>
#include <string.h>
#include "photoalbum.h"
#include "my_memory_checker_216.h"

Photo *create_photo(int id, const char *description) {
    Photo *photo;

    photo = malloc(sizeof(Photo));
    if (photo == NULL) {
        free(photo);
        return NULL;
    }

    photo->id = id;

    if (description == NULL) {
        photo->description = NULL;
    } else {
        photo->description = malloc(strlen(description) + 1);
        if (photo->description == NULL) {
            free(photo->description);
            return NULL;
        }
    }

    strcpy(photo->description, description);

    return photo;
}

void print_photo(Photo *photo) {

    if (photo == NULL) { return; }

    if (photo->description == NULL) {
        printf("Photo Id: %d, Description: None\n", photo->id);
    } else {
        printf("Photo Id: %d, Description: %s\n", photo->id, photo->description);
    }
}

void destroy_photo(Photo *photo) {

    if (photo == NULL) { return; }

    free(photo->description);
    free(photo);
}

void initialize_album(Album *album) {

    if (album == NULL) { return; }

    album->size = 0;
}

void print_album(const Album *album) {
    int i;

    if (album == NULL) { return; }

    if (album->size == 0) {
        printf("Album has no photos.\n");
    }

    for (i = 0; i < album->size; i++) {
        print_photo(album->all_photos[i]);
    }
}

void destroy_album(Album *album) {
    int i;

    if (album == NULL) { return; }

    for (i = 0; i < album->size; i++) {
        destroy_photo(album->all_photos[i]);
    }

    album->size = 0;
}

void add_photo_to_album(Album *album, int id, const char *description) {
    int index;

    if (album == NULL) { return; }

    index = album->size;

    if (index < MAX_ALBUM_SIZE) {
        album->all_photos[index] = create_photo(id, description);
        album->size++;
    }
}