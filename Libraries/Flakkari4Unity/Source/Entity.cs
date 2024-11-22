using System;
using UnityEngine;

using CurrentProtocol = Flakkari4Unity.Protocol.V1;

namespace Flakkari4Unity.ECS
{
    public class Entity : MonoBehaviour
    {
        public ulong Id { get; set; }

        public void CreateComponents(byte[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                switch ((CurrentProtocol.ComponentId)data[i])
                {
                    case CurrentProtocol.ComponentId.CHILD:
                        i++;
                        var child = gameObject.AddComponent<Components.Common.Child>();
                        child.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.EVOLVE:
                        i++;
                        var evolve = gameObject.AddComponent<Components.Common.Evolve>();
                        evolve.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.HEALTH:
                        i++;
                        var health = gameObject.AddComponent<Components.Common.Health>();
                        health.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.ID:
                        i++;
                        Id = BitConverter.ToUInt64(data, i);
                        i += sizeof(ulong);
                        break;

                    case CurrentProtocol.ComponentId.LEVEL:
                        i++;
                        var level = gameObject.AddComponent<Components.Common.Level>();
                        level.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.PARENT:
                        i++;
                        var parent = gameObject.AddComponent<Components.Common.Parent>();
                        parent.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TAG:
                        i++;
                        var tag = gameObject.AddComponent<Components.Common.Tag>();
                        tag.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TEMPLATE:
                        i++;
                        var template_ = gameObject.AddComponent<Components.Common.Template>();
                        template_.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TIMER:
                        i++;
                        var timer = gameObject.AddComponent<Components.Common.Timer>();
                        timer.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.WEAPON:
                        i++;
                        var weapon = gameObject.AddComponent<Components.Common.Weapon>();
                        weapon.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TRANSFORM_3D:
                        i++;
                        var existingTransform = gameObject.GetComponent<Transform>();
                        existingTransform.position = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
                        i += sizeof(float) * 3;
                        existingTransform.rotation = new Quaternion(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2), BitConverter.ToSingle(data, i + sizeof(float) * 3));
                        i += sizeof(float) * 4;
                        existingTransform.localScale = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
                        i += sizeof(float) * 3;
                        break;

                    case CurrentProtocol.ComponentId.MOVABLE_3D:
                        i++;
                        var movable = gameObject.AddComponent<Components._3D.Movable>();
                        movable.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.CONTROL_3D:
                        i++;
                        var control = gameObject.AddComponent<Components._3D.Control>();
                        control.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.BOXCOLLIDER:
                        i++;
                        var boxCollider = gameObject.AddComponent<Components._3D.BoxCollider>();
                        boxCollider.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.SPHERECOLLIDER:
                        i++;
                        var sphereCollider = gameObject.AddComponent<Components._3D.SphereCollider>();
                        sphereCollider.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.RIGIDBODY_3D:
                        i++;
                        var rigidbody = gameObject.GetComponent<Components._3D.RigidBody>() ?? gameObject.AddComponent<Components._3D.RigidBody>();
                        rigidbody.Deserialize(data, ref i);
                        break;
                }
            }
        }

        public void UpdateComponents(byte[] data)
        {
            for (int i = 0; i < data.Length; i++)
            {
                switch ((CurrentProtocol.ComponentId)data[i])
                {
                    case CurrentProtocol.ComponentId.CHILD:
                        i++;
                        var child = gameObject.GetComponent<Components.Common.Child>();
                        child.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.EVOLVE:
                        i++;
                        var evolve = gameObject.GetComponent<Components.Common.Evolve>();
                        evolve.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.HEALTH:
                        i++;
                        var health = gameObject.GetComponent<Components.Common.Health>();
                        health.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.LEVEL:
                        i++;
                        var level = gameObject.GetComponent<Components.Common.Level>();
                        level.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.PARENT:
                        i++;
                        var parent = gameObject.GetComponent<Components.Common.Parent>();
                        parent.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TAG:
                        i++;
                        var tag = gameObject.GetComponent<Components.Common.Tag>();
                        tag.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TEMPLATE:
                        i++;
                        var template_ = gameObject.GetComponent<Components.Common.Template>();
                        template_.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TIMER:
                        i++;
                        var timer = gameObject.GetComponent<Components.Common.Timer>();
                        timer.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.WEAPON:
                        i++;
                        var weapon = gameObject.GetComponent<Components.Common.Weapon>();
                        weapon.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.TRANSFORM_3D:
                        i++;
                        var transform = gameObject.GetComponent<Transform>();
                        transform.position = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
                        i += sizeof(float) * 3;
                        transform.rotation = new Quaternion(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2), BitConverter.ToSingle(data, i + sizeof(float) * 3));
                        i += sizeof(float) * 4;
                        transform.localScale = new Vector3(BitConverter.ToSingle(data, i), BitConverter.ToSingle(data, i + sizeof(float)), BitConverter.ToSingle(data, i + sizeof(float) * 2));
                        i += sizeof(float) * 3;
                        break;

                    case CurrentProtocol.ComponentId.MOVABLE_3D:
                        i++;
                        var movable = gameObject.GetComponent<Components._3D.Movable>();
                        movable.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.CONTROL_3D:
                        i++;
                        var control = gameObject.GetComponent<Components._3D.Control>();
                        control.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.BOXCOLLIDER:
                        i++;
                        var boxCollider = gameObject.GetComponent<Components._3D.BoxCollider>();
                        boxCollider.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.SPHERECOLLIDER:
                        i++;
                        var sphereCollider = gameObject.GetComponent<Components._3D.SphereCollider>();
                        sphereCollider.Deserialize(data, ref i);
                        break;

                    case CurrentProtocol.ComponentId.RIGIDBODY_3D:
                        i++;
                        var rigidbody = gameObject.GetComponent<Components._3D.RigidBody>();
                        rigidbody.Deserialize(data, ref i);
                        break;
                }
            }
        }
    }
}
