//
// Singleton模板
// 参考 boost 1-36-0/pool/detail/singleton.hpp
// 说明 http://www.yulefox.com/index.php/20081119/design-patterns-03.html/
//

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <typename T> 
struct singleton_default
{ 
public: 
    typedef T object_type; 
    static object_type& instance() 
    { 
        static object_type obj; 
        create_object.do_nothing();
        return obj; 
    } 

private: 
    struct object_creator 
    { 
        object_creator() { singleton_default<T>::instance(); }
        inline void do_nothing() const {} 
    }; 
    static object_creator create_object; 
    singleton_default(); 
}; 

template <typename T> 
typename singleton_default<T>::object_creator 
singleton_default<T>::create_object;

#define INSTANCE(class_name) singleton_default<class_name>::instance()

#endif // _SINGLETON_H_
