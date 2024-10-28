#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <sqlite3.h>
#include <sstream>

using namespace std;

// Función para decodificar los parámetros URL
string urldecode(const string& str) {
    string result;
    char ch;
    int i, ii;
    for (i = 0; i < str.length(); i++) {
        if (str[i] == '+') {
            result += ' ';  // Reemplazar + por espacio
        } else if (int(str[i]) == 37) {  // % indica codificación hexadecimal
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            result += ch;
            i = i + 2;
        } else {
            result += str[i];
        }
    }
    return result;
}


static int funcionConLaRespuesta(void *data, int argc, char **argv, char **azColName) {
    string *html = static_cast<string*>(data);

    if (html->find("<th>") == string::npos) {
        *html += "<table border=\"1\" class=\"table table-bordered mt-5\"><tr>";
        for (int i = 0; i < argc; i++) {
            *html += "<th>" + string(azColName[i]) + "</th>";
        }
        *html += "</tr>";
    }

    *html += "<tr>";
    for (int i = 0; i < argc; i++) {
        *html += "<td>" + string(argv[i] ? argv[i] : "NULL") + "</td>";
    }
    *html += "</tr>";

    return 0;
}

int main() {

    // Obtener el método de la solicitud (GET o POST)
    const char* request_method = getenv("REQUEST_METHOD");
    char* query_string = nullptr;

    if (request_method != nullptr && strcmp(request_method, "GET") == 0) {
        query_string = getenv("QUERY_STRING");
    } 
    else if (request_method != nullptr && strcmp(request_method, "POST") == 0) {
        const char* content_length_str = getenv("CONTENT_LENGTH");
        int content_length = 0;
        if (content_length_str != nullptr) {
            content_length = atoi(content_length_str);
        }
        query_string = new char[content_length + 1];
        cin.read(query_string, content_length);
        query_string[content_length] = '\0';
    }

    // Inicializar variables
    string sql;
    if (query_string != nullptr) {
        string query = urldecode(query_string);
        size_t pos = query.find("q=");
        if (pos != string::npos) {
            sql = query.substr(pos + 2);
        }
    }

    sqlite3* db;
    string html;

    sqlite3_open("basededatos.db", &db);

    if (sql.empty()) {
        html = "Consulta SQL vacía.";
    } else {
        char* errMsg = 0;
        int rc = sqlite3_exec(db, sql.c_str(), funcionConLaRespuesta, &html, &errMsg);

        if (rc != SQLITE_OK) {
            html = "Error en la ejecución de la consulta: ";
            html += errMsg;
            sqlite3_free(errMsg);
        } else {
            html += "</table>";
        }
    }

    cout << "Content-Type: text/plain\n\n";
    cout << html << endl;

    sqlite3_close(db);
    delete[] query_string;

    return 0;
}
