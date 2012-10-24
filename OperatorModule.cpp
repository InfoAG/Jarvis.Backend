template <class InterfaceT, class StaticsT>
unsigned int OperatorModule<InterfaceT, StaticsT>::priority() const
{
    if (interface.priority == nullptr) return statics.priority;
    else return interface.priority();
}

template <class InterfaceT, class StaticsT>
QDataStream &operator<<(QDataStream &stream, const OperatorModule<InterfaceT, StaticsT> &module)
{
    stream << module.name() << module.description();
    if (module.interface.matches == nullptr) stream << static_cast<quint8>(0) << *module.statics.matches;
    else stream << static_cast<quint8>(1);
    if (module.interface.priority == nullptr) stream << static_cast<quint8>(0) << module.statics.priority;
    else stream << static_cast<quint8>(1);
    stream << static_cast<quint8>(module.statics.needsParseForMatch);
    return stream;
}
