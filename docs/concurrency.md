# Concurrency 🎀

Собственный фреймворк для работы с многопоточным кодом, реализованный при прохождении [курса](https://gitlab.com/Lipovsky/concurrency-course/-/tree/master/) Романа Липовского.
Ниже описание только той части функционала, которая используется в проекте.

### Компоненты:

- Thread pool / scheduler
- Stackful [fibers](https://graphitemaster.github.io/fibers/) с примитивами синхронизации
- Functional [futures](http://dist-prog-book.com/chapter/2/futures.html)

## Thread pool / scheduler 🤹‍♀️

Пул потоков для координации/парковки потоков, планирования/распределения задач между ними. 

Реализованы два вида пулов для потоков: [для тяжелых вычислений](../lib/concurrency/executors/tp/compute), [Go-планировщик](../lib/concurrency/executors/tp/fast) Дмитрия Вьюкова ([подробнее](https://www.youtube.com/watch?v=-K11rY57K7k)).
В проекте используется только второй.

### API:

#### 👉 `Start`

Пул конструируется от числа потоков, после чего необходимо вызвать Start:

```cpp
tp::ThreadPool pool{/*threads=*/4};

// Стартуем 4 потока-воркера
pool.Start();
```

#### 👉 `Submit`

Пул исполняет задачи без принимаемого и без возвращаемого значений. Запланировать их можно при помощи асинхронного Submit:

```cpp
tp::ThreadPool pool{4};

pool.Start();

pool.Submit([] {
  fmt::println("Running on thread pool");
});
```

#### 👉 `WaitIdle`

Вызов WaitIdle блокирует поток, пока все запланированные в пул задачи не будут исполнены:

```cpp
tp::ThreadPool pool{4};

pool.Start();

pool.Submit([] {
  std::this_thread::sleep_for(1s);
  tp::ThreadPool::Current()->Submit([] {
    fmt::println("Executed");
  });
});

// К моменту вызова WaitIdle вложенная задача скорее всего еще не была запланирована на исполнение.
// Но вызов WaitIdle дождется ее завершения.
pool.WaitIdle();
// <-- На экране напечатано "Executed"
```

#### 👉 `Stop`

Stop предназначен для остановки пула. Вызов этого метода гарантирует, что новых задач больше не будет.


## Stackful Fibers 🚄

[Файберы](../lib/concurrency/fibers) - легковесные аналоги потоков, представляют собой обертки над stackful корутинами, планируемыми и исполняемыми
в tread pool. Предназначены для кооперативной многозадачности.

### API:

#### 👉 `Go`
Конструктор для файберов, планирует процедуру на исполнение в файбере в заданный (текущий) планировщик:

```cpp
tp::ThreadPool pool{4};

for (size_t i = 0; i < 256; ++i) {
    // Запускаем файбер в пуле-планировщике
    fibers::Go(scheduler, [] {
    for (size_t j = 0; j < 3; ++j) {
      // Уступаем поток планировщика другому файберу
      fibers::Yield();
    }
  
    // Запускаем файбер в том же пуле
    fibers::Go([] {
      fibers::Yield();
    }
    });
  });
} 
```

### Примитивы синхронизации:
Для написания многопоточного кода для файберов также реализованы примитивы синхронизации,
не блокирующие потоки пула в точках ожидания. Среди них: Event, WaitGroup и Mutex, используемый в проекте.


## Functional Futures 🔮

[Фьючи](../lib/concurrency/futures) предоставляют функциональную модель concurrency. В основе лежит понятие _future_ - будущий, еще не готовый, результат асинхронной операции.
Их мы будем комбинировать в функциональном стиле.

#### `Result`
Любая операция может завершиться ошибкой, поэтому результат асинхронной операции
будет представлен контейнером Result - оберткой над [tl::expected](https://github.com/TartanLlama/expected).

#### `Operator Pipe |`
Для комбинирования фьюч перегружен operator |

### Конструкторы

#### 👉 `Contract`

Создает асинхронный контракт между producer и consumer:

```cpp
auto [f, p] = futures::Contract<int>();

// Producer
std::move(p).SetValue(7);

// Consumer
auto r = std::move(f) | futures::Get();
```

#### 👉 `Submit`

Конструирует фьючу, представляющую вычисление в пуле потоков:

```cpp
auto f = futures::Submit(pool, [] {
  return result::Ok(7);  // Упаковываем значение в Result
);
```

#### 👉 `Submit`

Фьюча, представляющая значение Unit (аналог void):

```cpp
auto f = futures::Just()
    | futures::Map([](Unit) {
        return 42;
      });
```

### Комбинаторы

#### 👉 `Map`

Сигнатура ([нотация](https://www.haskell.org/tutorial/functions.html) заимствована из функциональных языков): 
- `Future<T> → (T → U) → Future<U>` 

Применяет функцию к значению входящей фьючи, если она выполнилась успешно. Иначе, прокидывает ошибку:

```cpp
auto f = futures::Value(1) | futures::Map([](int v) {
  return v + 1;
});
```

#### 👉 `AndThen` / `OrElse`

Сигнатура:

- AndThen: `Future<T>  → (T → Result<U>) → Future<U>`
- OrElse: `Future<T> → (Error → Result<T>) → Future<T>`

Представляют асинхронный аналог try + catch:

```cpp
auto f = futures::Failure<int>(TimeoutError()) | 
    | futures::AndThen([](int v) {
        return result::Ok(v + 1);
      })
    | futures::OrElse([](Error) {
        return result::Ok(7);  // Fallback
      });
```

### Терминаторы

#### 👉 `Get`

Блокирует текущий поток до готовности результата:

```cpp
// Планируем задачу в пул потоков и дожидаемся результата
Result<int> r = futures::Submit(pool, [] {
  return result::Ok(7);
}) | futures::Get();
```

#### 👉 `Detach`

Detach поглощает фьючу и игнорирует результат:

```cpp
// Завершение задачи в пуле нас не интересует
futures::Submit(pool, [] {
  /* ... */
}) | futures::Detach();
```

#### 👉 `Await`

Неблокирующий аналог Get для файберов:

```cpp
fibers::Go(pool, [] {
  auto r = futures::Submit(pool, [] {
    return result::Ok(7);
  }) | futures::Await(); // <- При вызове поток пула не блокируется
})
```


## Зависимости ⛓️


- [Twist](https://gitlab.com/Lipovsky/twist) – фреймворк для тестирования многопоточного кода
- [Sure](https://gitlab.com/Lipovsky/sure) – механизм переключения контекста исполнения
- [Wheels](https://gitlab.com/Lipovsky/wheels) – библиотека общих компонент
- [function2](https://github.com/Naios/function2) – замена для `std::function`
- [expected](https://github.com/TartanLlama/expected) – `expected<T, E>`

