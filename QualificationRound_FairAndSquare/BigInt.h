#include <algorithm>
#include <array>
#include <cstdint>
#include <limits>


template<bool signed = false, std::uint32_t requestedBytes = 0>
struct BigInt
{
	typedef uint64_t BigIntElement;
	static_assert(requestedBytes % sizeof(BigIntElement) == 0, "Requested bytes must be a multiple of BigIntElement size");

	enum
	{
		BigIntElementCount = sizeof(BigIntElement / requestedBytes);
		BigIntElementMax = std::numeric_limits<BigIntElement>::max();
	};

	std::array<BigIntElement, BigIntElementCount> m_elements;

	BigInt(const BigInt& other)
	{
		std::copy(other.m_elements.begin(), other.m_elements.end(), m_elements.begin());
	}

	const BigInt& operator=(const BigInt& other)
	{
		if(this != &other)
		{
			std::copy(other.m_elements.begin(), other.m_elements.end(), m_elements.begin());
		}
		return *this;
	}


	/// Operators
	// --- Arithematic boolean operators ---
	/*BigInt  operator+ (const BigInt& other)
	{
		BigInt toReturn(other);
		BigIntElement carry = 0;
		std::transform(	m_elements.rbegin(), 
						m_elements.rend(), 
						toReturn.m_elements.rbegin(),
						toReturn.m_elements.rbegin(), 
						[&](BigIntElement lhs, BigIntElement rhs) -> BigIntElement
		{
			

		});
	}
    BigInt  operator+ (BigIntElement other);
    BigInt  operator* (BigInt other) const;
    BigInt  operator* (BigIntElement other) const;
    BigInt  operator/ (const BigInt& other) const;
    BigInt  operator% (const BigInt& other) const;
    BigInt& operator+= (const BigInt& other);
    BigInt  operator++ (int);
    BigInt& operator++ ();
    BigInt  operator-  (const BigInt& other);
    BigInt  operator-  ();
    BigInt  operator-- (int);
    BigInt& operator-- ();
    BigInt& operator-= (const BigInt& other);

    // --- Bitwise boolean operators ---
    BigInt  operator&   (const BigInt& other);
    BigInt  operator|   (const BigInt& other);
    BigInt  operator^   (const BigInt& other);
    BigInt& operator&=  (const BigInt& other);
    BigInt& operator|=  (const BigInt& other);
    BigInt& operator^=  (const BigInt& other);
    BigInt  operator~   ();
    BigInt  operator>>  (std::size_t shift);
    BigInt  operator<<  (std::size_t shift);
    BigInt& operator>>= (std::size_t shift);
    BigInt& operator<<= (std::size_t shift);

    // --- Comparison operators ---
    bool operator== (const BigInt& other) const;
    bool operator!= (const BigInt& other) const;
    bool operator<  (const BigInt& other) const;
    bool operator>  (const BigInt& other) const;
    bool operator<= (const BigInt& other) const;
    bool operator>= (const BigInt& other) const;*/
};
