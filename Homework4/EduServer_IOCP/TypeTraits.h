#pragma once
#include <type_traits>
#include <memory>

template <typename T>
struct is_shared_ptr
{
	const static bool value = false;
};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T> >
{
	const static bool value = true;
};
