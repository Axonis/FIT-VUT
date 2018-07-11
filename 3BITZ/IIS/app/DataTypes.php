<?php

namespace App;

class DataTypes
{
    public static $zavaznost = ['High', 'Medium', 'Low'];
    public static $vulner = ['System', 'Module', 'View'];
    public static $typ_bugu = ['Logic', 'Syntax', 'Not a Bug', 'Interfacing', 'Resource',  'Arithmetic'];
    public static $produkt = ['OpenStack', 'Libvirt', 'OpenShift', 'WildFly'];
    public static $ticket_type = ['New', 'Assigned', 'Open', 'Pending', 'Testing', 'Closed'];
    public static $user_types = ['User', 'Programmer', 'Admin'];
    public static $languages = ['C', 'C++', 'D', 'Python', 'Java', 'PHP', 'C#'];
}