<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Illuminate\Support\Collection;
use App\Ticket;
use App\Description;
use App\Module;
use Illuminate\Support\Facades\Auth;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    /**
     * Show the application dashboard.
     *
     * @return \Illuminate\Http\Response
     */
    public function index()
    {
        $tickets = Ticket::where('stav', 'New')->get();
        $my_tickets = Ticket::whereNotIn('stav', ['Closed'])->
        where('name_prideleny', Auth::user()->name)->get();

        $num_of_tickets = sizeof($tickets);
        $num_of_my_tickets = sizeof($my_tickets);

        $new_tickets_my_modules = Ticket::where('stav', 'New')->get();
        $bugs_on_my_module = Module::select('tickets.id_ticket', 'tickets.popis')->
            where('name_zodpovedny', Auth::user()->name)->
            join('bugs', 'bugs.id_modul', '=', 'modules.id_modul')->
            join('descriptions', 'descriptions.id_bug', '=', 'bugs.id_bug')->
            join('tickets', 'tickets.id_ticket', '=', 'descriptions.id_ticket')->
            where('tickets.stav', 'New')->
            groupBy('id_ticket')->
            get();

        if($num_of_tickets == 1)
            $count_all = "ticket";
        else
            $count_all = "tickets";

        if($num_of_my_tickets == 1)
            $count_assigned = "ticket";
        else
            $count_assigned = "tickets";

        if(sizeof($bugs_on_my_module) == 1)
            $count_module = "ticket";
        else
            $count_module = "tickets";


        return view('home', compact('tickets', 'num_of_tickets', 'my_tickets', 'num_of_my_tickets',
            'bugs_on_my_module', 'count_all', 'count_assigned', 'count_module'));
    }
}
