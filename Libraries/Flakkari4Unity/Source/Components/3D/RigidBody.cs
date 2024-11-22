using System;
using UnityEngine;

namespace Flakkari4Unity.ECS.Components._3D
{
    public class RigidBody : UnityEngine.Rigidbody
    {
        internal void Deserialize(byte[] data, ref int i)
        {
            mass = BitConverter.ToSingle(data, i);
            i += sizeof(float);
            drag = BitConverter.ToSingle(data, i);
            i += sizeof(float);
            angularDrag = BitConverter.ToSingle(data, i);
            i += sizeof(float);

            int bitIndex = 0;
            useGravity = (data[i] & (1 << (bitIndex % 8))) != 0;
            bitIndex++;
            isKinematic = (data[i] & (1 << (bitIndex % 8))) != 0;

            i += sizeof(byte);
        }
    }
}
