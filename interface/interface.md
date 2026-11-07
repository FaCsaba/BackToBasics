# Interface

Interfaces are a promise that something is able to do a certain task. For example, say every struct that we malloc should be able to be freed.
If we make that promise that a struct is capable of being freed, we have an interface on our hands. Let's call these Objects.

```c
interface Object {
    void free();
}

struct Animal { ... };
// we promise that Animal conforms to Object
void animal_free(Animal* this) {
    ...
}

struct String { ... };
void string_free(String* this) {
    ...
}
```

## Why is this useful?

Let's say you want to play music from a file. Depending on it's format you have to read it differently and play it, but all you really care about is the fact that it should be played.

```c
interface MusicPlayer {
    void play();
    void stop();
    void seek();
}

struct FlacMusicPlayer { ... };
struct OggMusicPlayer { ... };
struct MP3MusicPlayer { ... };

MusicPlayer getMusicPlayer(char* file) {
    switch (get_file_extension(file)) {
    case ".flac": return (FlacMusicPlayer) { ... };
    case ".ogg": return (OggMusicPlayer) { ... };
    case ".mp4": return (MP3MusicPlayer) { ... };
    }
}

MusicPlayer mp = getMusicPlayer("something-something.mp3");
music_player_play(mp);
```

We, and most importantly the compiler, knows that the `play()` function is available on `mp`.

This is called function dispatching. When you have multiple types of "objects" and the function that ends up being used is based on the objects' type. 

There are multiple ways to achieve this exact behavior. The easiest of which is:

## Enum Unions
or discriminated unions or just unions. It has many names but the main idea is this:

```c
// The available types are explicitly listed 
enum MusicPlayerType {
    MusicPlayerTypeFlac,
    MusicPlayerTypeOgg,
    MusicPlayerTypeMP3,
};

// We make a struct that holds both the type of the music player and the fields of all of the music players.
// Because a union is only as big as the biggest item it holds, it doesn't take up a huge amount of space
struct MusicPlayer {
    MusicPlayerType type;
    union as {
        struct FlacMusicPlayer { ... } flac,
        struct OggMusicPlayer { ... } ogg,
        struct Mp3MusicPlayer { ... } mp3,
    };
};

void music_player_play(MusicPlayer mp) {
    // We can switch (dispatch) on the current type and play the music in a way that these formats require
    switch mp.type {
    case MusicPlayerTypeFlac: ...;
    case MusicPlayerTypeOgg: ...;
    case MusicPlayerTypeMP3: ...;
    };
}
```

## Virtual Method Table

Let's say you are a library author and you want users to be able to extend on the amount of MusicTypes your library offers. Currently that's not possible (unless user's directly change your source-code), because the `music_player_play()` function is only set up to handle the currently defined `MusicPlayerType`s and no more.

To be more flexible we can turn to Virtual Method Tables, or more commonly known as __vtables__. The idea is the following:

```c
// We construct a table of functions that are callable on objects like this
struct MusicPlayerVTable {
    // This is a pointer to a function with the return as void and the first argument a void pointer.
    // The void pointer argument is very important as it will point to the object that this function is being called on.
    // This of it as the `this` pointer in most languages. 
    void (*play)(void* this);
    void (*stop)(void* this);
    void (*seek)(void* this, int pos);
};

struct MusicPlayer {
    MusicPlayerVTable* vtable;
};

void music_player_play(MusicPlayerVTable* music_player) {
    music_player->play(music_player)
}

// Every implementation has a pointer to the VTable
struct FlacMusicPlayer {
    // It's very important that the `base` is the first field, because methods like `music_player_play` expected it to be the first item. 
    MusicPlayer base;
    ...
};
// In c++ and most languages with inheritance, this table is constructed for you, in c, it's manual (as most things :>)
MusicPlayerVTable FlacMusicPlayerVTable = { .play = flac_music_player_play, .stop = flac_music_player_stop, .seek = flac_music_player_seek };

struct OggMusicPlayer {
    MusicPlayer base;
    ...
};
struct MP3MusicPlayer {
    MusicPlayer base;
    ...
};

int main(void) {
    // When creating these structs, it's important to remember to populate the vtable field
    FlacMusicPlayer fmp = FlacMusicPlayer { .base = &FlacMusicPlayerVTable, ... };
    // It's safe to cast, because the beginning layout of `FlacMusicPlayer` is the exact same as `MusicPlayer`
    MusicPlayer *mp = (MusicPlayer*) &fmp;
    music_player_play(mp);
}
```

## Runtime constructed VTables

For more flexibility we can construct the VTables ourselves at runtime. This involves creating an object which stores a pointer to the original struct along with function pointers. This way we can call function by calling the function pointer stored on the object with the pointer to the original struct.

