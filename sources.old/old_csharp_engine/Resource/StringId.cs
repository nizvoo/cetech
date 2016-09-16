﻿namespace CETech.Resource
{
    /// <summary>
    ///     String id
    /// </summary>
    public static partial class StringId64
    {
        /// <summary>
        ///     Get stringid from string
        /// </summary>
        /// <param name="str">String</param>
        /// <returns>String id</returns>
        public static long FromString(string str)
        {
            return FromStringImpl(str);
        }
    }
}