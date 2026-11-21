#pragma once

namespace REPENTOGON {
	template <typename T>
	struct Ok
	{
		T value;
	};

	template <typename E>
	struct Err
	{
		E error;
	};

	template <typename T>
	Ok<T> ok(T v)
	{
		return Ok<T>{v};
	}

	template <typename E>
	Err<E> err(E e)
	{
		return Err<E>{e};
	}

	template<typename T, typename E>
	class Result
	{
		std::variant<Ok<T>, Err<E>> data;

	public:

		Result(Ok<T> v) : data(v) {}
		Result(Err<E> e) : data(e) {}

		bool is_ok() const
		{
			return std::holds_alternative<Ok<T>>(data);
		}

		bool is_err() const
		{
			return std::holds_alternative<Err<E>>(data);
		}

		const T& unwrap() const
		{
			return std::get<Ok<T>>(data).value;
		}

		T& unwrap()
		{
			return std::get<Ok<T>>(data).value;
		}

		const E& unwrap_err() const
		{
			return std::get<Err<E>>(data).error;
		}

		E& unwrap_err()
		{
			return std::get<Err<E>>(data).error;
		}

	};
}