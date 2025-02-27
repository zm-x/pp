#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 2
#define INCREMENTS 10000

pthread_mutex_t mutex;  // تعريف الميوتيكس
int counter = 0;        // المتغير المشترك

// دالة لزيادة العداد
void *increment_counter(void *arg) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&mutex);  // قفل الميوتيكس
        counter++;                    // زيادة العداد
        pthread_mutex_unlock(&mutex); // فتح الميوتيكس
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // تهيئة الميوتيكس
    pthread_mutex_init(&mutex, NULL);

    // إنشاء الخيوط
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, increment_counter, NULL);
    }

    // الانتظار حتى تنتهي جميع الخيوط
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // تدمير الميوتيكس
    pthread_mutex_destroy(&mutex);

    // طباعة القيمة النهائية للعداد
    printf("Final counter value: %d\n", counter);
    return 0;
}
