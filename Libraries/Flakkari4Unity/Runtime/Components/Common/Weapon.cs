using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components.Common
{
    public class Weapon : MonoBehaviour
    {
        public ulong minDamage;
        public ulong maxDamage;
        public float chargeTime = 0;
        public float chargeMaxTime;
        public bool isCharging = false;
        public float timeSinceLastShot = 0;
        public float fireRate;
        public ulong level;

        internal void Deserialize(byte[] data, ref int i)
        {
            minDamage = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            maxDamage = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
            chargeMaxTime = BitConverter.ToSingle(data, i);
            i += sizeof(float);
            fireRate = BitConverter.ToSingle(data, i);
            i += sizeof(float);
            level = BitConverter.ToUInt64(data, i);
            i += sizeof(ulong);
        }
    }
}
