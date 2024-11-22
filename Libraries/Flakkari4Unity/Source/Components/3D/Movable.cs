using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components._3D
{
    public class Movable : MonoBehaviour
    {
        public Vector3 velocity;
        public Vector3 acceleration;
        public float minSpeed;
        public float maxSpeed;

        internal void Deserialize(byte[] data, ref int i)
        {
            velocity = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            acceleration = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
            i += sizeof(float) * 3;
            minSpeed = BitConverter.ToSingle(data, i);
            i += sizeof(float);
            maxSpeed = BitConverter.ToSingle(data, i);
            i += sizeof(float);
        }
    }
}
