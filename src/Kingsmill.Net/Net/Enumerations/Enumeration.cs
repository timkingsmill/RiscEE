using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Kingsmill.Net.Enumerations
{
    public abstract class Enumeration : IComparable
    {
        protected Enumeration(int id, string name) => (Id, Name) = (id, name);

        public string Name { get; private set; }
        public int Id { get; private set; }
        public override string ToString() => Name;

        public static IEnumerable<T> GetAll<T>() where T : Enumeration =>
            typeof(T).GetFields(BindingFlags.Public |
                                BindingFlags.Static |
                                BindingFlags.DeclaredOnly)
                     .Select(f => f.GetValue(null))
                     .Cast<T>();

        public int CompareTo(object? other)
        {
            if (other is not Enumeration otherValue)
            {
                return -1;
            }
            return Id.CompareTo(((Enumeration)otherValue).Id);
        }

        public override bool Equals(object? obj)
        {
            if (obj is not Enumeration otherValue)
            {
                return false;
            }
            var typeMatches = GetType().Equals(obj.GetType());
            var valueMatches = Id.Equals(otherValue.Id);
            return typeMatches && valueMatches;
        }

        public override int GetHashCode()
        {
            throw new NotImplementedException();
        }

        #region Operator Overloads

        public static bool operator == (Enumeration left, Enumeration right)
        {
            if (left is null)
            {
                return right is null;
            }
            return left.Equals(right);
        }

        public static bool operator != (Enumeration left, Enumeration right)
        {
            return !(left == right);
        }

        public static bool operator < (Enumeration left, Enumeration right)
        {
            return left is null ? right is not null : left.CompareTo(right)<0;
        }

        public static bool operator <= (Enumeration left, Enumeration right)
        {
            return left is null || left.CompareTo(right) <= 0;
        }

        public static bool operator > (Enumeration left, Enumeration right)
        {
            return left is not null && left.CompareTo(right) > 0;
        }

        public static bool operator >= (Enumeration left, Enumeration right)
        {
            return left is null ? right is null : left.CompareTo(right) >= 0;
        }

        #endregion
    }
}
