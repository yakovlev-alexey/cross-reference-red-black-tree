
<b>Введение. Общая постановка задачи</b>

<i>1.2.4. Перекрестные ссылки. Красно-черное дерево.</i>

  Разработать и реализовать алгоритм формирования перекрестных ссылок:

    • прочитать текст и вывести его с добавлением последовательных номеров строк;
    • собрать все слова, встречающиеся в тексте;
    • сформировать таблицу, в которой все слова будут расположены в алфавитном порядке и для каждого слова будет указан список строк его нахождения (по возрастанию номеров строк).

  Для реализации задания использовать красно-черное дерево. Узел красно-черного дерева должен содержать:

    • Ключ –слово,
    • Информационная часть – ссылка на список, содержащий номера строк.
    
<b>Описание алгоритма решения и используемых структур данных</b>

  Красно-чёрное дерево — двоичное дерево поиска, в котором каждый узел имеет атрибут цвет. При этом:

    1. Узел может быть либо красным, либо чёрным и имеет двух потомков.
    2. Корень — чёрный. Это правило слабо влияет на работоспособность модели, так как цвет корня всегда можно изменить с красного на чёрный.
    3. Все листья — чёрные и не содержат данных.
    4. Оба потомка каждого красного узла — чёрные.
    5. Любой простой путь от узла-предка до листового узла-потомка содержит одинаковое число чёрных узлов.
    
  Благодаря этим ограничениям, путь от корня до самого дальнего листа не более чем вдвое длиннее, чем до самого ближнего и дерево примерно сбалансировано. Операции вставки, удаления и поиска требуют в худшем случае времени, пропорционального длине дерева, что позволяет красно-чёрным деревьям быть более эффективными в худшем случае, чем обычные двоичные деревья поиска.

  Новый узел в красно-чёрное дерево добавляется на место одного из листьев, представляемыми абстрактными черными листьями, не содержащими информации, окрашивается в красный цвет. Далее в зависимости от получившегося дерева возможны несколько случаев:

    1. Текущий узел – корень, тогда он перекрашивается в черный цвет. Таким образом во все  пути в текущем дереве добавляется один черный узел и свойство 5 не нарушается.
    2. Предок текущего узла – черный. Тогда свойства не нарушаются, потому что новый узел – красный – заменяет черный и путь до его листьев не изменяет количество черных узлов.
    3. Родитель и дядя вставляемого узла – красные. Тогда их оба можно перекрасить в черный, а дедушку можно перекрасить в красный. Теперь вставляемый узел удовлетворяет второму случаю, но дедушка мог нарушить свойства 2 и 4. Чтобы это исправить, процедура выбора случая выполняется рекурсивно для дедушки.
    4. Родитель текущего узла – красный, но дядя – черный. Также текущий узел правый потомок родительского узла, а родитель левый потомок дедушки. Тогда следует выполнить поворот дерева, который поменяет ролями текущий узел и его родителя. Теперь нарушено свойство 4, для исправления этого нарушения требуется рассмотреть случай 5 для бывшего родительского элемента. При этом не будут нарушены другие свойства (количество черных узлов в пути к поддеревьям вставляемого и родительского элемента не поменялось, так как они оба красные). Аналогично для случаев с инвертированными направлениями (лево — право).
    5. Родитель текущего узла красный, но дядя — черный. Текущий узел левый потомок родителя, который в свою очередь левый потомок своего предка. В таком случае требуется выполнить поворот дерева на узле-дедушке текущего. Тогда получится, что дедушка станет потомком родителя текущего элемента. Бывший дедушка должен был быть черным для удовлетворения 4 свойству. Тогда нужно поменять цвета дедушки и родительского элемента. Свойство 4 будет удовлетворено, а свойство 5 не будет нарушено, так как все пути, что проходили через черного дедушку, будут проходить через черного родителя, количество черных узлов не изменилось.
    
<b>Описание спецификации программы</b>

  Программа должна представлять собой консольное приложение, предоставляющее пользователю возможность создать словарь перекрестных ссылок для текстового файла, указав его название. Пользователь может получить результат прямо в окне консольного приложения, либо указать название файла, в который будет произведена запись. 

  При формировании словаря перекрестных ссылок должны учитываться все слова, записанные буквами английского алфавита или цифрами, а также их комбинациями. Любые знаки препинания не должны учитываются, например, анализ строки “.word-abc?>.”, должен результировать только в слова word и abc. Регистр букв не должен учитываться, например, строки “woRd” и “Word” результируют в одно и то же слово word. Если слово встречается в ряде строк, то номера строк должны быть перечислены вместе в порядке возрастания, разделенные пробелом. Вывод самих слов осуществляется в алфавитном порядке.

  Также пользователь может использовать приложение для нумерации строк в файле, результат нумерации будет выведен в отдельный файл. При этом нумерация должна вестись таким же образом, что и при формировании словаря перекрестных ссылок. 
  В случае невозможности работы с указанными файлами (например, при отсутствии указанного файла или невозможности его открыть, а также совпадении имен входного и выходного файлов для нумерации) пользователю должно быть выведено предупреждение, а программа должна прекратить свое выполнение.

<b>Описание программы</b>

<i>Файлы, функции и сущности:</i>

<i>Файл map.hpp:</i>

  Объявление и имплементация шаблонного класса Map, представляющего собой словарь с использованием красно-черного дерева. Шаблон имеет аргументы K — тип данных ключа, V — тип данных хранимого значения, Comparator — функтор, указываемый опционально, выполняющий сравнение ключей узлов. По умолчанию Comparator принимает значение std::less<K>. Интерфейс класса имеет следующие открытые (публичные) методы: конструктор по умолчанию, метод contains(), возвращающий булевое значение true, если переданный в него ключ находится в данном дереве, оператор индексации, принимающий значение ключа и возвращающий хранимое значение в узле, с таким ключом. Если такого узла нет, будет выброшено исключение. Оператор индексации позволяет также перезаписывать хранимые значения (возвращает ссылку на l-value). Для добавления новых пар ключ-значение в дерево, применяется функция insert(). Для получения всех узлов дерева применяется публичные классы итератора iterator и const_iterator с перегруженными операторами инкремента и декремента и методами для доступа к ключу и значению, хранимым в текущем узле.

  Имплементация класса достигается с помощью структуры в стиле С node_t, хранящей пару ключ-значение,  а также указателей на двух потомков (слева и справа) и собственного предка. Для поиска информации об узле применяются скрытые функции для поиска "дедушки", "брата" и "дяди" указанного узла. Для добавления применяется ряд последовательно и рекурсивно вызываемых функций, рассматривающих различные случаи восстановления корректного состояния красно-черного дерева, а также функции поворота дерева. Сам объект дерева хранит только указатель на корень дерева и объект функтора сравнения, скрытые в специальном объекте для удобства описания методов класса.

<i>Файл list.hpp:</i>

  Объявление и имплементация класса List, представляющий собой односвязный шаблонный список без повторений. Индексация осуществляется с помощью классов итераторов iterator и const_iterator, вставка методом push_back(). Данная имплементация итератора позволяет в полной мере пользоваться функциями высшего порядка библиотеки <functional>, например, std::for_each(), предоставляя удобный способ итерации по списку.

  Имплементация достигается с помощью структуры в стиле С node_t, хранящей значение и ссылку на следующий элемент. Сам список хранит только указатель на начало, скрытый в специальном объекте для удобства описания методов класса.

<i>Файл text-analyzer.hpp и text-analyzer.cpp:</i>

  Объявление и имплементация класса TextAnalyzer, обязанность которого заключается в чтении файла и формирования таблицы слов и номеров строк, в которых они встречаются. Объект класса создается конструктором по умолчанию. Для формирования словаря перекрестных ссылок применяется метод analyze, получающий на вход название файла или входной поток, из которого будет совершаться чтение. Для вывода полученной таблицы применяется метод printAnalysis, принимающий на вход название файла или выходной поток, в который будет совершаться запись. Метод getDictonary() позволяет иметь доступ к полученному словарю перекрестных ссылок после вызова метода analyze. При повторном анализе старый словарь удаляется. Имеется вспомогательная статичная функция enumerateLines, которая читает инфорамцию из входного потока или файла и выводит в другой выходной поток или файл с пронумерованными строками. Подсчет строк идет тем же методом, что и при анализе.

  Имплементация класса достигается с помощью объекта словаря Map с ключом-строкой и значением – списком без повторений list. Во время анализа текст читается построчно, каждая строка анализируется с помощью регулярного выражения /[a-zA-Z0-9]+/, позволяющего определить все слова и числа в тексте без лишних символов.

<i>Файл main.cpp:</i>

  Имплементация функции main для использования функционала класса TextAnalyzer с помощью консоли. Приложение будет предлагать пользователю проанализировать текст и составить словарь перекрестных ссылок или пронумеровать строки в тексте. Словарь будет предложены вывести в консоль или в файл, в то время как нумерация строк предлагается только для вывода в файл. При ошибке ввода или ошибке при работе с файлами в поток для ошибок будет выведено сообщение с описанием ошибки и приложение прекратит свое выполнение с кодом ошибки 1.

<i>Файл test-main.cpp:</i>

  Имплементация автоматических тестов с помощью библиотеки С++ BOOST для проверки базового функционала формирования перекрестных ссылок.

<i>Компиляция проекта</i>

  Компиляция может осуществляться с помощью средства CMake. В репозитории git и приложениях отчета имеется файла CMakeLists.txt, описывающий цель компиляции ConsoleTextAnalyzer. Компиляцию и запуск с помощью терминала из папки проекта с использованием CMake и GNU Make можно осуществить следующим образом:
  
    $ mkdir build & cd build
    $ cmake ..
    $ make ConsoleTextAnalyzer
    $ ./ConsoleTextAnalyzer
    $ ...

<i>Формат входных данных</i>

  На вход программе может подаваться любой текстовый файл.

<i>Формат выходных данных</i>

  Таблица с двумя столбцами. Первый столбец – слова, второй – номера строк, где встречается соответствующее слово. Ширина первого столбца выбирается относительно самого длинного слова или шапки таблицы.

<b>Заключение</b>

  В результате выполнения лабораторной работы мною была изучена структура данных красно-черного дерево, а также алгоритмы, используемые для работы с ним. В процессе создания решения были разработаны классы, представляющие собой словарь на основе красно-черного дерева и односвязный список без повторений. В решении были использованы такие продвинутые конструкции языка С++ как шаблоны, итераторы, функторы, функции высшего порядка, регулярные выражения, автоматические тесты с помощью библиотеки BOOST. Приложение, полученное в результате разработки, соответствует поставленным целям и установленным спецификациям.

<b>Список источников</b>

    1. James Paton. "Red-Black Trees". // University of Wisconsin-Madison.- URL: http://pages.cs.wisc.edu/~paton/readings/Red-Black-Trees/.
    2. Morris, John . "Red–Black Trees" // Data Structures and Algorithms (1998).- URL: https://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html.