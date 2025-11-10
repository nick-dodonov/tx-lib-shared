// #include <emscripten.h>
// #include <stdio.h>

// EM_ASYNC_JS(int, do_fetch, (), {
//     const response = await fetch("https://example.com/");
//     const text = await response.text();
//     console.log(text);
//     return text.length;
// });

// int main() {
//     printf("Before fetch\n");
//     int result = do_fetch();  // БЛОКИРУЕТСЯ здесь, пока не завершится
//     printf("After fetch, result: %d\n", result);
//     return 0;
// }

#include <emscripten/fetch.h>
#include <stdio.h>
#include <string.h>

void onSuccess(emscripten_fetch_t* fetch) {
    printf("Downloaded %llu bytes\n", fetch->numBytes);
    printf("Data: %.*s\n", (int)fetch->numBytes, fetch->data);
    emscripten_fetch_close(fetch);
    
    // Продолжаем выполнение после получения данных
    // Можете вызвать другую функцию отсюда
}

void onError(emscripten_fetch_t* fetch) {
    printf("Download failed: %d\n", fetch->status);
    emscripten_fetch_close(fetch);
}

int main() {
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onSuccess;
    attr.onerror = onError;
    
    emscripten_fetch(&attr, "https://example.com/");
    
    // main() завершается немедленно, но callbacks будут вызваны позже
    return 0;
}