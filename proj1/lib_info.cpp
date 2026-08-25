// Cole Ruth and Kerry Cao
// aug 20
// Lib_info : given data about a library of music, organizes all songs by album and artist in 
//            a map, then prints out a formatted view of the entire library.

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

struct Song {
    string name;
    int time;
};

struct Album {
    string name;
    map<int, Song> songs;
    int time;
    int nsongs;
};

struct Artist {
    string name;
    map<string, Album> albums;
    int time;
    int nsongs;
};

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "one argument expected" << endl;
        return 1;
    }

    ifstream fin;
    fin.open(argv[1]);

    map<string, Artist> library;

    // read in info from the file

    while (true) {

        string title;
        fin >> title;

        if (!fin) break;

        replace(title.begin(), title.end(), '_', ' ');

        string time;
        fin >> time;

        // convert time to seconds

        size_t colon_position = time.find(':');
        string minutes = time.substr(0, colon_position);
        string seconds = time.substr(colon_position + 1);
        int duration_seconds = (stoi(minutes) * 60) + stoi(seconds);

        // replace underscores with spaces in the artist, album, and genre names

        string artist;
        fin >> artist;
        replace(artist.begin(), artist.end(), '_', ' ');

        string album;
        fin >> album;
        replace(album.begin(), album.end(), '_', ' ');

        string genre;
        fin >> genre;
        replace(genre.begin(), genre.end(), '_', ' ');

        int track;
        fin >> track;

        // look up to see if the artist already exists
        if (library.find(artist) == library.end()) {
            // it doesn't exist create new artist
            Artist artist_struct;
            artist_struct.name = artist;
            artist_struct.time = duration_seconds;
            artist_struct.nsongs = 1;

            Album album_struct;
            album_struct.name = album;
            album_struct.time = duration_seconds;
            album_struct.nsongs = 1;

            Song song_struct;
            song_struct.name = title;
            song_struct.time = duration_seconds;

            album_struct.songs[track] = song_struct;
            
            artist_struct.albums[album] = album_struct;

            library[artist] = artist_struct;
        } else {
            // the artist does exist add to that artist

            // check if the album exists
            if (library[artist].albums.find(album) == library[artist].albums.end()) {
                // it doesn't exist

                Album album_struct;
                album_struct.name = album;
                album_struct.time = duration_seconds;
                album_struct.nsongs = 1;

                Song song_struct;
                song_struct.name = title;
                song_struct.time = duration_seconds;

                album_struct.songs[track] = song_struct;
                
                library[artist].albums[album] = album_struct;
            } else {
                // the album does exist

                // add the song
                Song song_struct;
                song_struct.name = title;
                song_struct.time = duration_seconds;

                library[artist].albums[album].songs[track] = song_struct;
                library[artist].albums[album].time += duration_seconds;
                library[artist].albums[album].nsongs++;
            }

            library[artist].time += duration_seconds;
            library[artist].nsongs++;
        }
    }

    // print everything out

    map<string, Artist>::iterator artist_it;
    for (artist_it = library.begin(); artist_it != library.end(); ++artist_it) {

        // output artist

        Artist artist = (*artist_it).second;

        cout << artist.name << ": " << artist.nsongs << ", " << (artist.time / 60) << ":" << setw(2) << setfill('0') << (artist.time % 60) << endl;

        map <string, Album>::iterator album_it;

        // output albums

        for (album_it = artist.albums.begin(); album_it != artist.albums.end(); ++album_it) {

            Album album = (*album_it).second;

            cout << "        " << album.name << ": " << album.nsongs << ", " << (album.time / 60) << ":" << setw(2) << setfill('0') << (album.time % 60) << endl;

            map <int, Song>::iterator song_it;

            // output songs

            for (song_it = album.songs.begin(); song_it != album.songs.end(); ++song_it) {

                int track_num = (*song_it).first;
                Song song = (*song_it).second;

                cout << "                " << track_num << ". " << song.name << ": " << (song.time / 60) << ":" << setw(2) << setfill('0') << (song.time % 60) << endl;
            }
        }
    }
}