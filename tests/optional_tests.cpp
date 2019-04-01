#include "gtest/gtest.h"

#include "optional"

#include <string>
#include <vector>
#include <cstring>

using hs::optional;
using hs::nullopt;
using hs::in_place;

TEST(Optional, ConstructorEmpty)
{
	optional<int> i;
	EXPECT_FALSE(i);
	optional<int> j = nullopt;
	EXPECT_FALSE(j);
	optional<int> k{};
	EXPECT_FALSE(k);
}


optional<std::string> emptyOptional()
{
	return optional<std::string>();
}

TEST(Optional, CopyConstructor)
{
	optional<std::string> emptySource;
	optional<std::string> fullSource{ "Content" };

	optional<std::string> emptyTarget(emptySource);
	EXPECT_FALSE(emptyTarget);

	optional<std::string> fullTarget(fullSource);
	EXPECT_TRUE(fullTarget);
	EXPECT_EQ("Content", *fullTarget);
}

TEST(Optional, MoveConstructor)
{
	optional<std::string> emptySource;
	optional<std::string> fullSource{ "Content" };

	optional<std::string> emptyTarget(std::move(emptySource));
	EXPECT_FALSE(emptyTarget);

	optional<std::string> fullTarget(std::move(fullSource));
	EXPECT_TRUE(fullTarget);
	EXPECT_EQ("Content", *fullTarget);
}


TEST(Optional, ConvertingCopyConstructor)
{
	optional<const char*> emptySource;
	optional<const char*> fullSource{ "Content" };

	optional<std::string> emptyTarget(emptySource);
	EXPECT_FALSE(emptyTarget);

	optional<std::string> fullTarget(fullSource);
	EXPECT_TRUE(fullTarget);
	EXPECT_EQ("Content", *fullTarget);
}

TEST(Optional, ConvertingMoveConstructor)
{
	optional<const char*> emptySource;
	optional<const char*> fullSource{ "Content" };

	optional<std::string> emptyTarget(std::move(emptySource));
	EXPECT_FALSE(emptyTarget);

	optional<std::string> fullTarget(std::move(fullSource));
	EXPECT_TRUE(fullTarget);
	EXPECT_EQ("Content", *fullTarget);
}


TEST(Optional, InPlaceConstructor)
{
	struct X
	{
		X(int a, int b) : a_{ a }, b_{ b } {};
		int a_;
		int b_;
	};

	optional<X> x{ in_place, 2, 3 };
	EXPECT_EQ(2, x->a_);
	EXPECT_EQ(3, x->b_);
}

TEST(Optional, InPlaceInitializerList)
{
	optional<std::vector<int>> opt(in_place, { 1, 2, 3, 4 });
	EXPECT_TRUE(opt);
	EXPECT_EQ(4, opt->size());
}


TEST(Optional, ConvertingConstructor)
{
	{
		optional<std::string> opt("char*");
		EXPECT_EQ("char*", *opt);
	}

	{
		optional<double> opt(1.2f);
		EXPECT_NEAR(1.2, *opt, 1e-4);
	}
}


TEST(Optional, DestructorNonTrivial)
{
	class X
	{
	public:
		X(bool& a) : destructed(a) {}
		~X()
		{
			destructed = true;
		}

		bool& destructed;
	};

	bool destructed{ false };
	{
		optional<X> opt(destructed);
	}

	EXPECT_TRUE(destructed);
}

TEST(Optional, AssgnmentNullopt)
{
	optional<int> a(5);
	EXPECT_TRUE(a);
	a = nullopt;
	EXPECT_FALSE(a);

	a = nullopt;
	EXPECT_FALSE(a);
}

TEST(Optional, AssignmentOperator)
{

	optional<int> emptySource;
	optional<int> filledSource1(5);
	optional<int> filledSource2(6);

	optional<int> target;

	EXPECT_FALSE(target);
	target = emptySource;
	EXPECT_FALSE(target);
	target = filledSource1;
	EXPECT_EQ(5, *target);
	target = filledSource2;
	EXPECT_EQ(6, *target);
	target = emptySource;
	EXPECT_FALSE(target);

}

TEST(Optional, MoveAssignmentOperator)
{

	optional<std::string> emptySource;
	optional<std::string> filledSource1("one");
	optional<std::string> filledSource2("two");

	optional<std::string> target;

	EXPECT_FALSE(target);
	target = std::move(emptySource);
	EXPECT_FALSE(target);
	target = std::move(filledSource1);
	EXPECT_EQ("one", *target);
	EXPECT_EQ("", *filledSource1);
	target = std::move(filledSource2);
	EXPECT_EQ("two", *target);
	EXPECT_EQ("", *filledSource2);
	target = emptySource;
	EXPECT_FALSE(target);

}

TEST(Optional, ForwardingReferenceAssignment)
{
	optional<double> opt;

	opt = 1.0f;
	EXPECT_NEAR(1.0, *opt, 1e-4);
	opt = static_cast<int>(2);
	EXPECT_NEAR(2.0, *opt, 1e-4);
}

TEST(Optional, ConvertingAssignmentOperator)
{

	optional<const char*> emptySource;
	optional<const char*> filledSource1("one");
	optional<const char*> filledSource2("two");

	optional<std::string> target;

	EXPECT_FALSE(target);
	target = std::move(emptySource);
	EXPECT_FALSE(target);
	target = std::move(filledSource1);
	EXPECT_EQ("one", *target);
	target = std::move(filledSource2);
	EXPECT_EQ("two", *target);
	target = emptySource;
	EXPECT_FALSE(target);

}

TEST(Optional, ConvertingMoveAssignmentOperator)
{
	// Need to come up with classes that convert and where I can verify moving
	optional<const char*> emptySource;
	optional<const char*> filledSource1("one");
	optional<const char*> filledSource2("two");

	optional<std::string> target;

	EXPECT_FALSE(target);
	target = std::move(emptySource);
	EXPECT_FALSE(target);
	target = std::move(filledSource1);
	EXPECT_EQ("one", *target);
	target = std::move(filledSource2);
	EXPECT_EQ("two", *target);
	target = emptySource;
	EXPECT_FALSE(target);

}

TEST(Optional, NonConstValue)
{
	optional<int> opt(1);
	EXPECT_EQ(1, opt.value());
	EXPECT_EQ(1, *opt);
	opt.value() = 2;
	EXPECT_EQ(2, opt.value());
	*opt = 3;
	EXPECT_EQ(3, opt.value());


	optional<int> empty;
	EXPECT_THROW(empty.value(), hs::bad_optional_access);
}

TEST(Optional, ConstValue)
{
	const optional<int> opt(3);
	EXPECT_EQ(3, opt.value());
	EXPECT_EQ(3, *opt);

	const optional<int> empty;
	EXPECT_THROW(empty.value(), hs::bad_optional_access);
}

TEST(Optional, RValue)
{
	optional<int> opt(3);
	EXPECT_EQ(3, std::move(opt).value());
	optional<int> empty;
	EXPECT_THROW(std::move(empty).value(), hs::bad_optional_access);
}

TEST(Optional, Swap)
{
	{
		optional<int> source(3);
		optional<int> target(4);

		source.swap(target);
		EXPECT_EQ(4, *source);
		EXPECT_EQ(3, *target);
	}

	{
		optional<int> source;
		optional<int> target(4);

		source.swap(target);
		EXPECT_FALSE(target);
		EXPECT_EQ(4, *source);
		source.swap(target);
		EXPECT_FALSE(source);
		EXPECT_EQ(4, *target);
	}
	{
		optional<int> source;
		optional<int> target;
		source.swap(target);

		EXPECT_FALSE(source);
		EXPECT_FALSE(target);
	}
}

TEST(Optional, Reset)
{
	optional<std::string> opt("Value");

	opt.reset();
	EXPECT_FALSE(opt);
}

TEST(Optional, DontHaveValue)
{
	optional<int> i;
	EXPECT_FALSE(i.has_value());
	EXPECT_FALSE(i);

	optional<int> j = hs::nullopt;
	EXPECT_FALSE(i.has_value());
	EXPECT_FALSE(i);
}

TEST(Optional, EmplaceArgs)
{
	struct X
	{
		X(int a, std::string b) : a(a), b(b) {}
		int a;
		std::string b;
	};

	optional<X> opt;
	auto x = opt.emplace(1, "test");

	EXPECT_TRUE(opt);
	EXPECT_EQ(1, opt->a);
	EXPECT_EQ(1, x.a);

	EXPECT_EQ("test", opt->b);
	EXPECT_EQ("test", x.b);
}

TEST(Optional, EmplaceInitList)
{
	struct X
	{

		X(std::vector<int> a, std::string b) : a(a), b(b) {}
		std::vector<int> a;
		std::string b;
	};

	optional<X> opt;
	auto x = opt.emplace({ 1, 2, 3 }, "test");
	EXPECT_TRUE(opt);
	EXPECT_EQ(3, opt->a.size());
	EXPECT_EQ(3, x.a.size());
	EXPECT_EQ("test", opt->b);
	EXPECT_EQ("test", x.b);

}

TEST(Optional, MoveAccess)
{
	optional<std::string> s(in_place, "test");
	EXPECT_EQ("test", *s);

	// From cppreference example page
	auto v = *std::move(s);
	EXPECT_EQ("", *s);
	EXPECT_EQ("test", v);
}

