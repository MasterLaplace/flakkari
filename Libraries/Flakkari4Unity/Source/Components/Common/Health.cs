using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Health : MonoBehaviour
    {
        public ulong currentHealth;
        public ulong maxHealth;
        public ulong shield;
        public ulong maxShield;

        internal void Deserialize(byte[] data, ref int i)
        {
            currentHealth = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            maxHealth = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            shield = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            maxShield = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
        }
    }
}
