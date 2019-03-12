# otus_homework_3_allocator
Особенности моей реализации такие:
    
1)	Мой аллокатор custom_allocator написан в стиле аллокаторов <b>С++17</b>, 
    соответственно мой контейнер custom_forward_list так же написан с поддержкой аллокаторов <b>С++17</b>, 
    т.е. вся работа с аллокатороми в контейнере идет через <b>allocator_traits<></b>
    Проблем со сборкой кода на travis не столкнулся, поскольку ранее уже перешел на gcc версии 8.
    
2)	В рамках домашки следующее не требовалось, но созданный аллокатор, позволяет:

        a.	Задавать в параметрах шаблона отдельно размер первоначально выделяемой памяти, 
            и размер последующих выделений памяти. По умолчанию и там и там 1 так, что по умолчанию 
            мой аллокатор ведет себя как аллокатор стандартной библиотеки.

        b.	Эти же два параметра, можно задавать через конструктор аллокатора, что имеет 
             приоритет над параметрами заданными через параметры шаблона. Что позволяет нам 
             оптимизировать выделения памяти в рантайме.

        c.	Так же размер последующих выделений памяти, можно поменять в рантайме, через 
             соответствующую ф-ию аллокатора, даже поле того, как аллокатор передан в контейнер и 
             контейнер начал его использовать. 

    Более подробно эти возможности можно посмотреть в тестах.

    Тестирование моего аллокатора через стандартный профилировщик Visual Studio 2017 показало, 
    что вставка в map<int,int> 100000 элементов на x64 съедает 4,5Mb оперативки против 11Mb со стандартным аллокатором.

    Хочу отметить, что Александреску я таки прочел, но уже после того, как код аллокатора был готов.

3)	Мой контейнер – это урезанный forward_list, который выполняет только те операции, 
    что требуются в домашнем задании. Проход по элементам контейнера сделал через итератор так, 
    что контейнер совместим с алгоритмами, которым требуется forward_iterator и циклом foreach.

4)	С факториалом получилась некоторая заминка. 
    
    Особенности работы программы ltrace в начале работы над кодом мне были не известны, 
    так что я решил, что неплохо будет прикрутить к домашнему заданию парочку дополнительных библиотек: 
      boost/math, boost/multiprecision, и GMP библиотеку для вычислений с плавающей запятой, целыми и 
      рациональными числами с произвольной точностью.
      
    Хотелось в рамках домашки протестировать библиотеки для расчетов чисел, выходящих за диапазон 
    стандартных типов С++.
    
    Но когда я уже все закончил и запустил команду <b>ltrace allocator</b>, стало понятно, что 
    вывод команды ltrace – совсем не читаемый. Так что я переделал вычисление факториала. 
    Добавив по умолчанию его наивную реализацию.
 
    Хотя вывод ltrace allocator  все же остается громоздким, теперь при вызове ltrace allocator все же 
    можно найти вызовы выделения памяти, что делает map и custom_forward_list с дефолтным аллокатором 
    стандартной библиотеки, и выделения памяти, что делает мой custom_allocator, через malloc.

    Наивную версию вычисления факториала, можно переключить на версию с boost/math, boost/multiprecision, и GMP, 
    если при запуске allocator задать дополнительный параметр: 
    
     <b>allocator -max_item <i>size</i></b>
     
    где <i>size</i> - это число элементов контейнера, т.е. другими словами, это то число, до которого мы будем считать факториалы.
    <i>size</i> - ничем не ограничен, только памятью компьютера и временем вычислений.


