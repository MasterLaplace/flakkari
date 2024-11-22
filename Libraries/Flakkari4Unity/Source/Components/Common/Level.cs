using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Level : MonoBehaviour
    {
        public ulong level;
        public string currentWeapon;
        public ulong currentExp;
        public ulong requiredExp;

        internal void Deserialize(byte[] data, ref int i)
        {
            level = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            ulong length = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            currentWeapon = System.Text.Encoding.UTF8.GetString(data, i, (int)length);
            i += (int)length;
            currentExp = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            requiredExp = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
        }
    }
}
